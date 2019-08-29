let style = ReactDOMRe.Style.make;
let addUnsafe = ReactDOMRe.Style.unsafeAddProp;
let combine = ReactDOMRe.Style.combine;

[%mui.withStyles
  "IdentityDialogStyles"(theme =>
    {
      identityDialog:
        style(
          ~width="90vw",
          ~margin="10vh auto",
          ~maxWidth="400px!important",
          ~padding=theme |> Utils.spacing(2),
          ~paddingTop=theme |> Utils.spacing(4),
          (),
        )
        ->MaterialUi.ThemeHelpers.addBreakpoint(
            ~theme,
            ~breakpoint=`MD,
            ~style=style(~width="50vw", ()),
          ),
      tab:
        style(
          ~minHeight=
            (theme->MaterialUi.Theme.Theme.spacingGet(1) * 3)
            ->Utils.px_of_int,
          (),
        ),
      title: style(~paddingLeft="0", ()),
      closeButton:
        style(
          ~position="absolute",
          ~right=theme |> Utils.spacing(1),
          ~top=theme |> Utils.spacing(1),
          (),
        ),
      tabRoot:
        style(
          ~marginBottom=theme |> Utils.spacing(2),
          ~minHeight=
            (theme->MaterialUi.Theme.Theme.spacingGet(1) * 3)
            ->Utils.px_of_int,
          (),
        ),
      /*** forms */
      submitButton: style(~marginTop=theme |> Utils.spacing(1), ()),
      formElement: style(~marginBottom=theme |> Utils.spacing(2), ()),
      errorMsg: style(~textAlign="center", ~fontSize="16px", ()),
      /*** providers */
      divider: style(~flex="1 1 auto", ()),
      providerSection:
        style(
          ~display="flex",
          ~alignItems="center",
          ~marginBottom=theme |> Utils.spacing(3),
          ~marginTop=theme |> Utils.spacing(3),
          (),
        ),
      userLink:
        style(
          ~textDecoration="underline",
          ~textTransform="none",
          ~padding="0",
          (),
        )
        ->addUnsafe(
            "&:hover",
            style(
              ~backgroundColor="transparent",
              ~textDecoration="underline",
              (),
            )
            ->Utils.styleToString,
          ),

      /*** user menu */
      userName: style(~fontWeight="500", ~width="100%", ()),
      userListItem: style(~flexWrap="wrap", ()),
    }
  )
];
