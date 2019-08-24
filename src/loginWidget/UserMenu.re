open UserTypes;

let style = ReactDOMRe.Style.make;
[%mui.withStyles
  "RootStyles"(theme =>
    {
      userName: style(~fontWeight="500", ~width="100%", ()),
      userListItem: style(~flexWrap="wrap", ()),
    }
  )
];

[@react.component]
let make = (~anchorEl, ~onClose) => {
  let identity = ReactNetlifyIdentity.useIdentityContext();
  let fullName =
    identity.user
    ->Belt.Option.flatMap(user => user.metaData)
    ->Belt.Option.map(data => data##full_name);

  let handleLogout = _ => {
    identity.logoutUser();
    ignore();
  };

  let classes = RootStyles.useStyles();

  <MaterialUi_Menu
    anchorEl
    open_={Belt.Option.isSome(anchorEl)}
    onClose
    getContentAnchorEl=None
    anchorOrigin={MaterialUi_Menu.AnchorOrigin.make(
      ~horizontal=`Enum(`Center),
      ~vertical=`Enum(`Bottom),
    )}
    transformOrigin={MaterialUi_Menu.TransformOrigin.make(
      ~horizontal=`Enum(`Center),
      ~vertical=`Enum(`Top),
    )}>
    {switch (fullName) {
     | None => React.null
     | Some(name) =>
       <MaterialUi_MenuItem className={classes.userListItem}>
         <MaterialUi_Typography component={`String("div")}>
           {React.string("Logged in as:")}
         </MaterialUi_Typography>
         <MaterialUi_Typography
           component={`String("div")} className={classes.userName}>
           {React.string(name)}
         </MaterialUi_Typography>
       </MaterialUi_MenuItem>
     }}
    <MaterialUi_Divider variant=`FullWidth />
    <MaterialUi_MenuItem onClick=handleLogout>
      <MaterialUi_ListItemIcon>
        <ReactFeather.LogoutIcon />
      </MaterialUi_ListItemIcon>
      <MaterialUi_ListItemText primary={React.string("Log out")} />
    </MaterialUi_MenuItem>
    <MaterialUi_Divider variant=`FullWidth />
    <MaterialUi_MenuItem>
      <MaterialUi_ListItemIcon>
        <ReactFeather.BookmarkIcon />
      </MaterialUi_ListItemIcon>
      <MaterialUi_ListItemText primary={React.string("My Library")} />
    </MaterialUi_MenuItem>
  </MaterialUi_Menu>;
};
