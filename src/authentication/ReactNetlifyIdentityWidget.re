module IdentityModal = {
  [@bs.module "react-netlify-identity-widget"] [@react.component]
  external make:
    (~showDialog: bool, ~onCloseDialog: unit => unit, unit) => React.element =
    "IdentityModal";
};

module IdentityContextProvider = {
  [@bs.module "react-netlify-identity-widget"] [@react.component]
  external make:
    (~url: string, ~children: React.element=?, unit) => React.element =
    "IdentityContextProvider";
};
