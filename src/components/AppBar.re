let isSearchPage = page =>
  switch (page) {
  | Routing.Search(_) => true
  | _ => false
  };

module Styles = {
  open Css;

  let desktopSearchBar = theme =>
    style([
      display(`none),
      media(
        Utils.getBreakpoint(`MD, theme),
        [
          display(`flex),
          flexGrow(1.0),
          flexShrink(1.0),
          flexBasis(`auto),
          justifyContent(`center),
          alignItems(`center),
        ],
      ),
    ]);

  let appBarSearchInput =
    style([
      maxWidth(px(400)),
      fontSize(rem(1.3)),
      position(`absolute),
      left(pct(50.0)),
      transform(translateX(pct(-50.0))),
    ]);

  let hideDesktop = theme =>
    style([media(Utils.getBreakpoint(`MD, theme), [display(`none)])]);

  let appBarIcons = style([marginLeft(`auto)]);
  let appName = theme =>
    style([
      fontWeight(`num(100)),
      fontSize(rem(1.5)),
      media(Utils.getBreakpoint(`MD, theme), [fontSize(rem(1.75))]),
    ]);
  let appIcon = theme =>
    style([marginLeft(px(theme |> Utils.spacingPx(1)))]);
};

[@react.component]
let make = (~isLoggedIn, ~activePage) => {
  let dispatch = AppCore.useDispatch();
  let theme = Mui_Theme.useTheme();
  let (userMenuAnchor, setUserMenuAnchor) = React.useState(() => None);

  React.useEffect1(
    () => {
      setUserMenuAnchor(_ => None);
      None;
    },
    [|isLoggedIn|],
  );

  <MaterialUi_AppBar color=`Default position=`Static>
    <MaterialUi_Container>
      <MaterialUi_Toolbar disableGutters=true>
        <RouterLink
          variant="h4"
          href="/"
          color=`Inherit
          className={Styles.appName(theme)}>
          {React.string("Podcast library")}
          <ReactFeather.MicIcon className={Styles.appIcon(theme)} />
        </RouterLink>
        {!isSearchPage(activePage)
           ? <div className={Styles.desktopSearchBar(theme)}>
               <SearchInput className=Styles.appBarSearchInput />
             </div>
           : React.null}
        <div className=Styles.appBarIcons>
          <RouterLink href={Routing.getUrlFromRoute(SearchEmpty)}>
            <MaterialUi_IconButton component={`String("span")}>
              <ReactFeather.SearchIcon />
            </MaterialUi_IconButton>
          </RouterLink>
          {isLoggedIn
             ? <>
                 <MaterialUi_IconButton
                   onClick={e => {
                     let anchor = e->ReactEvent.Mouse.currentTarget;
                     setUserMenuAnchor(_ => Some(anchor));
                   }}>
                   <ReactFeather.UserIcon />
                 </MaterialUi_IconButton>
                 <RouterLink href={Routing.getUrlFromRoute(MyLibrary)}>
                   <MaterialUi_IconButton component={`String("span")}>
                     <ReactFeather.BookmarkIcon />
                   </MaterialUi_IconButton>
                 </RouterLink>
               </>
             : <MaterialUi_Button
                 onClick={_ => dispatch(SetShowIdentityModal(true))}>
                 {React.string("Log in")}
               </MaterialUi_Button>}
          <UserMenu
            anchorEl=userMenuAnchor
            onLogout={() => Routing.pushRoute(SearchEmpty)}
            onClose={(_, _) => setUserMenuAnchor(_ => None)}
          />
        </div>
      </MaterialUi_Toolbar>
    </MaterialUi_Container>
  </MaterialUi_AppBar>;
};
