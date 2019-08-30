let style = ReactDOMRe.Style.make;
let addUnsafe = ReactDOMRe.Style.unsafeAddProp;
let combine = ReactDOMRe.Style.combine;

[%mui.withStyles
  "AppStyles"(theme =>
    {
      cardActions:
        style(
          ~padding=theme |> Utils.spacing(2),
          ~paddingTop="0px",
          ~justifyContent="flex-end",
          (),
        ),
      searchCard: style(~position="relative", ~overflow="initial", ()),

      cardSavedIcon:
        style(
          ~position="absolute",
          ~right=theme |> Utils.spacing(1),
          ~top=theme |> Utils.spacingTransform(1, space => space * (-1)),
          (),
        ),
      cardDescription: style(~overflow="hidden", ()),
      contentTabs: style(~marginBottom=theme |> Utils.spacing(2), ()),
      contentTab:
        style(~flex="1 1 0", ~maxWidth="none", ())
        ->MaterialUi.ThemeHelpers.addBreakpoint(
            ~theme,
            ~breakpoint=`MD,
            ~style=style(~flex="0", ()),
          ),
      cardLoadMoreBtn:
        style(
          ~marginTop=theme |> Utils.spacing(2),
          ~display="block",
          ~margin="0 auto",
          (),
        ),
      pageTitle: style(~fontWeight="100", ~textAlign="center", ()),
      pageContainer:
        style(~marginTop=theme |> Utils.spacing(4), ())
        ->MaterialUi.ThemeHelpers.addBreakpoint(
            ~theme,
            ~breakpoint=`MD,
            ~style=
              style(
                ~maxWidth="800px",
                ~marginLeft="auto",
                ~marginRight="auto",
                (),
              ),
          ),
      /*** notifications */
      successNotification:
        style(~backgroundColor=MaterialUi_Color_Green.c700, ()),
      errorNotification:
        style(
          ~backgroundColor=
            theme
            ->MaterialUi.Theme.Theme.paletteGet
            ->MaterialUi.Theme.Palette.errorGet
            ->MaterialUi.Theme.PaletteColor.mainGet,
          (),
        ),
      snackbarRoot:
        style(
          ~paddingLeft=theme |> Utils.spacing(1),
          ~paddingRight=theme |> Utils.spacing(1),
          (),
        ),
      snackbarContent: style(~flexGrow="0", ()),
      /*** search form */
      searchContainer:
        style(
          ~marginBottom=theme |> Utils.spacing(3),
          ~display="flex",
          ~flexDirection="row",
          (),
        ),
      searchFormControl: style(~maxWidth="600px", ~margin="0 auto", ()),
      searchInput: style(~fontSize="26px", ()),
      /*** app bar */
      desktopSearchBar:
        style(~display="none", ())
        ->MaterialUi.ThemeHelpers.addBreakpoint(
            ~theme,
            ~breakpoint=`MD,
            ~style=
              style(
                ~flex="1 1 auto",
                ~display="flex",
                ~justifyContent="center",
                ~alignItems="center",
                (),
              ),
          ),
      appBarSearchInput:
        style(
          ~maxWidth="400px",
          ~fontSize="1.3rem",
          ~position="absolute",
          ~left="50%",
          ~transform="translateX(-50%)",
          (),
        ),
      hideDesktop:
        style()
        ->MaterialUi.ThemeHelpers.addBreakpoint(
            ~theme,
            ~breakpoint=`MD,
            ~style=style(~display="none", ()),
          ),
      appBarIcons: style(~marginLeft="auto", ()),
      appName:
        style(~fontWeight="100", ~fontSize="1.5rem", ())
        ->MaterialUi.ThemeHelpers.addBreakpoint(
            ~theme,
            ~breakpoint=`MD,
            ~style=style(~fontSize="1.7rem", ()),
          ),
      appIcon: style(~marginLeft=theme |> Utils.spacing(1), ()),
    }
  )
];
