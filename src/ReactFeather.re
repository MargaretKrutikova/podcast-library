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
