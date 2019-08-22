let str = ReasonReact.string;
let style = ReactDOMRe.Style.make;
[%mui.withStyles
  "PageTitleStyles"(theme =>
    {
      root:
        style(
          ~fontWeight="100",
          ~textAlign="center",
          ~marginTop=theme |> Utils.spacing(3),
          (),
        ),
    }
  )
];

[@react.component]
let make = (~title) => {
  let classes = PageTitleStyles.useStyles();
  <MaterialUi_Typography
    className={classes.root}
    variant=`H3
    component={`String("h1")}
    gutterBottom=true>
    {React.string(title)}
  </MaterialUi_Typography>;
};
