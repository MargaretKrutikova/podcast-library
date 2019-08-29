open FormData;

let str = ReasonReact.string;

[@react.component]
let make = () => {
  let classes = IdentityDialogStyles.IdentityDialogStyles.useStyles();

  let (state, dispatch) = UserUtils.useReducerSafe(reducer, initState);
  let {email, password, fullName, status} = state;

  let identity = UserIdentity.Context.useIdentityContext();

  let handleSignup = _ => {
    dispatch(SubmitRequest);
    // data is of type UserIdentity.userMetaData
    identity.signupUser(~email, ~password, ~data={fullName: fullName})
    |> Js.Promise.then_(_ => dispatch(SubmitSuccess) |> Js.Promise.resolve)
    |> Js.Promise.catch(error =>
         dispatch(
           SubmitError(UserUtils.promiseErrorToJsObj(error)##message),
         )
         |> Js.Promise.resolve
       )
    |> ignore;
  };

  <form
    onSubmit={e => {
      ReactEvent.Form.preventDefault(e);
      handleSignup();
    }}>
    <MaterialUi_FormControl
      fullWidth=true classes=[Root(classes.formElement)]>
      <MaterialUi_FormControl
        fullWidth=true classes=[Root(classes.formElement)]>
        <MaterialUi_TextField
          autoFocus=true
          label={str("Name")}
          name="name"
          type_="text"
          fullWidth=true
          required=true
          value={`String(fullName)}
          disabled={status === Submitting}
          onChange={e => {
            let value = Utils.getInputValue(e);
            dispatch(SetFullName(value));
          }}
        />
      </MaterialUi_FormControl>
      <MaterialUi_TextField
        label={str("Email Address")}
        type_="email"
        fullWidth=true
        value={`String(email)}
        required=true
        name="email"
        disabled={status === Submitting}
        onChange={e => {
          let value = Utils.getInputValue(e);
          dispatch(SetEmail(value));
        }}
      />
    </MaterialUi_FormControl>
    <MaterialUi_FormControl
      fullWidth=true classes=[Root(classes.formElement)]>
      <MaterialUi_TextField
        label={str("Password")}
        type_="password"
        value={`String(password)}
        name="password"
        fullWidth=true
        required=true
        disabled={status === Submitting}
        onChange={e => {
          let value = Utils.getInputValue(e);
          dispatch(SetPassword(value));
        }}
      />
    </MaterialUi_FormControl>
    <MaterialUi_FormControl fullWidth=true className={classes.submitButton}>
      <MaterialUi_Button
        color=`Primary
        disabled={status === Submitting}
        variant=`Contained
        type_="submit"
        fullWidth=true>
        {str("Sign up")}
      </MaterialUi_Button>
    </MaterialUi_FormControl>
    {switch (status) {
     | Error(message) => <ErrorMessage message />
     | _ => React.null
     }}
  </form>;
};
