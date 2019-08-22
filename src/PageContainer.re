let style = ReactDOMRe.Style.make;
[%mui.withStyles
  "RootStyles"(theme =>
    {
      root:
        style()
        ->MaterialUi.ThemeHelpers.addBreakpoint(
            ~theme,
            ~breakpoint=`MD,
            ~style=style(~maxWidth="800px", ~margin="0 auto", ()),
          ),
    }
  )
];

[@react.component]
let make = (~children) => {
  let classes = RootStyles.useStyles();

  <div className={classes.root}> children </div>;
};
