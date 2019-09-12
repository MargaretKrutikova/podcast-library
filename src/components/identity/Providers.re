open IdentityDialogStyles;
let style = ReactDOMRe.Style.make;

[@react.component]
let make = () => {
  let identity = UserIdentity.Context.useIdentityContext();
  let theme = Mui_Theme.useTheme();

  <>
    <div className={Styles.providerSection(theme)}>
      <MaterialUi_Divider variant=`Middle className=Styles.divider />
      <MaterialUi_Typography variant=`Body2>
        {React.string("OR")}
      </MaterialUi_Typography>
      <MaterialUi_Divider variant=`Middle className=Styles.divider />
    </div>
    <div id="providers">
      {identity.settings.providers
       ->Belt.Array.map(provider =>
           <ProviderButton
             key={ReactNetlifyIdentity.providerToString(provider)}
             provider
             className={Styles.formElement(theme)}
           />
         )
       |> React.array}
    </div>
  </>;
};
