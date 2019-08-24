let style = ReactDOMRe.Style.make;
let addUnsafe = ReactDOMRe.Style.unsafeAddProp;
let combine = ReactDOMRe.Style.combine;

[%mui.withStyles
  "LoginDialogStyles"(theme =>
    {
      divider: style(~flex="1 1 auto", ()),
      providerSection:
        style(
          ~display="flex",
          ~alignItems="center",
          ~marginBottom=theme |> Utils.spacing(3),
          ~marginTop=theme |> Utils.spacing(3),
          (),
        ),
      formElement: style(~marginBottom=theme |> Utils.spacing(2), ()),
    }
  )
];

[@react.component]
let make = () => {
  let identity = ReactNetlifyIdentity.useIdentityContext();
  let classes = LoginDialogStyles.useStyles();

  <>
    <div className={classes.providerSection}>
      <MaterialUi_Divider variant=`Middle className={classes.divider} />
      <MaterialUi_Typography variant=`Body2>
        {React.string("OR")}
      </MaterialUi_Typography>
      <MaterialUi_Divider variant=`Middle className={classes.divider} />
    </div>
    {identity.settings.providers
     ->Belt.Array.map(provider =>
         <ProviderButton
           key={ReactNetlifyIdentity.providerToString(provider)}
           provider
           className={classes.formElement}
         />
       )
     |> React.array}
  </>;
};
