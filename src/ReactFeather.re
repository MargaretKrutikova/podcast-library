module SearchIcon = {
  [@bs.module "react-feather"] [@react.component]
  external make:
    (
      ~color: string=?,
      ~disabled: bool=?,
      ~onClick: 'c=?,
      ~size: string=?,
      ~className: string=?,
      unit
    ) =>
    React.element =
    "Search";
};

module UserIcon = {
  [@bs.module "react-feather"] [@react.component]
  external make:
    (
      ~color: string=?,
      ~disabled: bool=?,
      ~onClick: 'c=?,
      ~size: string=?,
      ~className: string=?,
      unit
    ) =>
    React.element =
    "User";
};

module BookmarkIcon = {
  [@bs.module "react-feather"] [@react.component]
  external make:
    (
      ~color: string=?,
      ~onClick: 'c=?,
      ~size: string=?,
      ~fill: string=?,
      ~className: string=?,
      unit
    ) =>
    React.element =
    "Bookmark";
};

module MicIcon = {
  [@bs.module "react-feather"] [@react.component]
  external make:
    (
      ~color: string=?,
      ~onClick: 'c=?,
      ~size: string=?,
      ~fill: string=?,
      ~className: string=?,
      unit
    ) =>
    React.element =
    "Mic";
};

module CloseIcon = {
  [@bs.module "react-feather"] [@react.component]
  external make:
    (
      ~color: string=?,
      ~onClick: 'c=?,
      ~size: string=?,
      ~fill: string=?,
      ~className: string=?,
      unit
    ) =>
    React.element =
    "X";
};

module LogoutIcon = {
  [@bs.module "react-feather"] [@react.component]
  external make:
    (
      ~color: string=?,
      ~onClick: 'c=?,
      ~size: string=?,
      ~fill: string=?,
      ~className: string=?,
      unit
    ) =>
    React.element =
    "LogOut";
};
