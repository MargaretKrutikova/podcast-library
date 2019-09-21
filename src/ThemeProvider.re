external styleToString: ReactDOMRe.Style.t => string = "%identity";

let theme =
  MaterialUi_Theme.create(
    MaterialUi_ThemeOptions.(
      make(
        ~palette=
          PaletteOptions.make(
            ~primary=Primary.make(~main="#20638f", ()),
            ~secondary=Secondary.make(~main="#669BBC", ()),
            ~background=TypeBackground.make(~default="#f3f3f3", ()),
            (),
          ),
        ~overrides=
          Overrides.make(
            ~muiTab=
              TabClassKey.make(
                ~root=
                  ReactDOMRe.Style.make(~borderBottom="2px solid #ddd", ()),
                (),
              ),
            ~muiIconButton=
              IconButtonClassKey.make(
                ~root=
                  ReactDOMRe.Style.make(~padding="8px", ~color="#1a5073", ()),
                (),
              ),
            ~muiLink=
              LinkClassKey.make(
                ~root=
                  ReactDOMRe.Style.make(~textDecoration="none", ())
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
                    ~backgroundColor="#34495e !important",
                    ~color="#fff",
                    ~boxShadow=
                      "0px 2px 6px 0px rgba(0,0,0,0.14), 0px 1px 12px 0px rgba(0,0,0,0.12)",
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
