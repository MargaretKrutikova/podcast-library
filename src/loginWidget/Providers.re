let style = ReactDOMRe.Style.make;

[@react.component]
let make = () => {
  let identity = UserIdentity.Context.useIdentityContext();
  let classes = IdentityDialogStyles.IdentityDialogStyles.useStyles();

  <>
    <div className={classes.providerSection}>
      <MaterialUi_Divider variant=`Middle className={classes.divider} />
      <MaterialUi_Typography variant=`Body2>
        {React.string("OR")}
      </MaterialUi_Typography>
      <MaterialUi_Divider variant=`Middle className={classes.divider} />
    </div>
    <div id="providers">
      {identity.settings.providers
       ->Belt.Array.map(provider =>
           <ProviderButton
             key={ReactNetlifyIdentity.providerToString(provider)}
             provider
             className={classes.formElement}
           />
         )
       |> React.array}
    </div>
  </>;
};
