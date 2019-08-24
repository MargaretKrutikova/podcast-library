let style = ReactDOMRe.Style.make;
let addUnsafe = ReactDOMRe.Style.unsafeAddProp;
let combine = ReactDOMRe.Style.combine;

[%mui.withStyles
  "LoginDialogStyles"(theme =>
    {
      forgotPasswordLink:
        style(
          ~textDecoration="underline",
          ~margin="0 auto",
          ~display="block",
          (),
        ),
      submitButtonElement: style(~marginTop=theme |> Utils.spacing(1), ()),
      formElement: style(~marginBottom=theme |> Utils.spacing(2), ()),
    }
  )
];

let str = ReasonReact.string;

[@react.component]
let make = () => {
  let classes = LoginDialogStyles.useStyles();
  let (email, setEmail) = React.useState(() => "");
  let (password, setPassword) = React.useState(() => "");

  let (loading, setLoading) = React.useState(() => false);

  let identityContext = ReactNetlifyIdentity.useIdentityContext();

  let handleLogin = _ => {
    setLoading(_ => false);
    identityContext.loginUser(~email, ~password, ~remember=true, ())
    |> Js.Promise.then_(_ => setLoading(_ => false) |> Js.Promise.resolve)
    |> Js.Promise.catch(_ => setLoading(_ => false) |> Js.Promise.resolve);

    ignore();
  };

  <form>
    <MaterialUi_FormControl
      fullWidth=true classes=[Root(classes.formElement)]>
      <MaterialUi_TextField
        autoFocus=true
        label={str("Email Address")}
        type_="email"
        fullWidth=true
        value=email
        onChange={e => {
          let value = Utils.getInputValue(e);
          setEmail(_ => value);
        }}
      />
    </MaterialUi_FormControl>
    <MaterialUi_FormControl
      fullWidth=true classes=[Root(classes.formElement)]>
      <MaterialUi_TextField
        label={str("Password")}
        type_="password"
        fullWidth=true
        value=password
        onChange={e => {
          let value = Utils.getInputValue(e);
          setPassword(_ => value);
        }}
      />
    </MaterialUi_FormControl>
    <MaterialUi_FormControl
      fullWidth=true
      className={classes.submitButtonElement}
      classes=[Root(classes.formElement)]>
      <MaterialUi_Button
        color=`Primary variant=`Contained fullWidth=true onClick=handleLogin>
        {str("Log in")}
      </MaterialUi_Button>
    </MaterialUi_FormControl>
    <MaterialUi_Link
      color=`Primary
      component={`String("button")}
      className={classes.forgotPasswordLink}>
      {str("Forgot password")}
    </MaterialUi_Link>
  </form>;
};