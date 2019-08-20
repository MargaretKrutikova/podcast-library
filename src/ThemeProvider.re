let theme =
  MaterialUi_Theme.create(
    MaterialUi_ThemeOptions.(
      make(
        ~palette=
          PaletteOptions.make(
            ~primary=Primary.make(~main="#29335C", ()),
            ~secondary=Secondary.make(~main="#669BBC", ()),
            (),
          ),
        (),
      )
    ),
  );

[@react.component]
let make = (~children) =>
  MaterialUi.(<MuiThemeProvider theme> children </MuiThemeProvider>);
