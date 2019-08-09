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

type userMetadata = {. "full_name": Js.Nullable.t(string)};

type identityUser = {
  .
  "user_metadata": Js.Nullable.t(userMetadata),
  "id": string,
  "email": string,
};

type identityContextValue = {
  .
  "user": Js.Nullable.t(identityUser),
  "isLoggedIn": Js.Nullable.t(bool),
};

[@bs.module "react-netlify-identity-widget"]
external useIdentityContext: unit => Js.Nullable.t(identityContextValue) =
  "useIdentityContext";
