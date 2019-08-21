external styleToString: ReactDOMRe.Style.t => string = "%identity";
let theme =
  MaterialUi_Theme.create(
    MaterialUi_ThemeOptions.(
      make(
        ~palette=
          PaletteOptions.make(
            ~primary=Primary.make(~main="#29335C", ()),
            ~secondary=Secondary.make(~main="#669BBC", ()),
            ~background=TypeBackground.make(~default="#F9F6F5", ()),
            (),
          ),
        ~overrides=
          Overrides.make(
            ~muiLink=
              LinkClassKey.make(
                ~root=
                  ReactDOMRe.Style.make(~textDecoration="none !important", ())
                  ->ReactDOMRe.Style.unsafeAddProp(
                      "&:hover",
                      ReactDOMRe.Style.make(~color="#29335C", ())
                      ->styleToString,
                    ),
                (),
              ),
            ~muiAppBar=
              AppBarClassKey.make(
                ~root=
                  ReactDOMRe.Style.make(
                    ~backgroundColor="#ECE0DB !important",
                    ~boxShadow=
                      "0px 1px 0px 0px rgba(0,0,0,0.2), 0px 2px 6px 0px rgba(0,0,0,0.14), 0px 1px 10px 0px rgba(0,0,0,0.12)",
                    (),
                  ),
                (),
              ),
            (),
          ),
        (),
      )
    ),
  );

[@react.component]
let make = (~children) =>
  MaterialUi.(<MuiThemeProvider theme> children </MuiThemeProvider>);
