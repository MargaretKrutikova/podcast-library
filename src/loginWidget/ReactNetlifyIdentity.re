external cast: Js.t({..}) => Js.Json.t = "%identity";
external castToObj: Js.Json.t => Js.t({..}) = "%identity";

type provider =
  | Bitbucket
  | GitHub
  | GitLab
  | Google;

let providerToString = provider =>
  switch (provider) {
  | Bitbucket => "bitbucket"
  | GitHub => "github"
  | GitLab => "gitlab"
  | Google => "google"
  };

type externalSettingsJs = {
  .
  "bitbucket": bool,
  "email": bool,
  "facebook": bool,
  "github": bool,
  "gitlab": bool,
  "google": bool,
};

[@bs.deriving abstract]
type settingsJs = {
  [@bs.as "external"]
  external_: externalSettingsJs,
  autoconfirm: bool,
  disable_signup: bool,
};

type settings = {
  autoconfirm: bool,
  disableSignup: bool,
  providers: array(provider),
};

let addProvider = (provider, isOn, providers) =>
  if (isOn) {
    Belt.Array.concat(providers, [|provider|]);
  } else {
    providers;
  };

let fromSettingsJs = settings => {
  let jsProviders = external_Get(settings);
  let providers =
    [||]
    |> addProvider(Bitbucket, jsProviders##bitbucket)
    |> addProvider(GitHub, jsProviders##github)
    |> addProvider(GitLab, jsProviders##gitlab)
    |> addProvider(Google, jsProviders##google);

  {
    autoconfirm: autoconfirmGet(settings),
    disableSignup: disable_signupGet(settings),
    providers,
  };
};

type userJs = {
  .
  "user_metadata": Js.Nullable.t(Js.Json.t),
  "id": string,
  "email": string,
  "role": string,
};

type user('a) = {
  metaData: option(Js.t({..}) as 'a),
  id: string,
  email: string,
  role: string,
};

let fromUserJs = (user: userJs) => {
  id: user##id,
  email: user##email,
  role: user##role,
  metaData:
    user##user_metadata
    ->Js.Nullable.toOption
    ->Belt.Option.map(data => castToObj(data)),
};

type reactNetlifyIdentityApiJs = {
  .
  "user": Js.Nullable.t(userJs),
  "isConfirmedUser": bool,
  "isLoggedIn": bool,
  [@bs.meth]
  "signupUser":
    (string, string, Js.Json.t) => Js.Promise.t(Js.Nullable.t(userJs)),
  [@bs.meth]
  "loginUser":
    (string, string, Js.Nullable.t(bool)) =>
    Js.Promise.t(Js.Nullable.t(userJs)),
  [@bs.meth] "logoutUser": unit => Js.Promise.t(Js.Nullable.t(userJs)),
  [@bs.meth] "requestPasswordRecovery": string => Js.Promise.t(unit),
  [@bs.meth]
  "recoverAccount": (string, Js.Nullable.t(bool)) => Js.Promise.t(userJs),
  [@bs.meth]
  "updateUser":
    {. "data": Js.Json.t} => Js.Promise.t(Js.Nullable.t(userJs)),
  [@bs.meth] "getFreshJWT": unit => Js.Promise.t(string),
  "_url": string,
  [@bs.meth] "loginProvider": string => unit,
  [@bs.meth] "acceptInviteExternalUrl": (string, string) => string,
  "settings": settingsJs,
};

type reactNetlifyIdentityApi('a) = {
  user: option(user('a)),
  isLoggedIn: bool,
  signupUser:
    (~email: string, ~password: string, ~data: Js.t({..}) as 'a) =>
    Js.Promise.t(option(user('a))),
  loginUser:
    (~email: string, ~password: string, ~remember: bool=?, unit) =>
    Js.Promise.t(option(user('a))),
  logoutUser: unit => Js.Promise.t(option(user('a))),
  requestPasswordRecovery: (~email: string) => Js.Promise.t(unit),
  recoverAccount:
    (~token: string, ~remember: bool=?, unit) => Js.Promise.t(user('a)),
  updateUser: Js.Json.t => Js.Promise.t(option(user('a))),
  getFreshJWT: unit => Js.Promise.t(string),
  url: string,
  loginProvider: provider => unit,
  acceptInviteExternalUrl: (~provider: provider, ~token: string) => string,
  settings,
};

module IdentityContextProvider = {
  [@bs.module "react-netlify-identity"] [@react.component]
  external make: (~url: string, ~children: React.element) => React.element =
    "IdentityContextProvider";
};

[@bs.module "react-netlify-identity"]
external useNetlifyIdentity: string => reactNetlifyIdentityApiJs =
  "useNetlifyIdentity";

[@bs.module "react-netlify-identity"]
external useIdentityContextJs: unit => reactNetlifyIdentityApiJs =
  "useIdentityContext";

let getUserMetadata = (user): Js.t({..} as 'a) =>
  castToObj(user##user_metadata);

let useIdentityContext = (): reactNetlifyIdentityApi('a) => {
  let identity = useIdentityContextJs();

  {
    user: identity##user->Js.Nullable.toOption->Belt.Option.map(fromUserJs),
    isLoggedIn: identity##isLoggedIn,
    signupUser: (~email, ~password, ~data) =>
      identity##signupUser(email, password, cast(data))
      |> Js.Promise.then_(user =>
           user->Js.Nullable.toOption->Belt.Option.map(fromUserJs)
           |> Js.Promise.resolve
         ),
    loginUser: (~email, ~password, ~remember=?, ()) =>
      identity##loginUser(email, password, Js.Nullable.fromOption(remember))
      |> Js.Promise.then_(user =>
           user->Js.Nullable.toOption->Belt.Option.map(fromUserJs)
           |> Js.Promise.resolve
         ),
    logoutUser: () =>
      identity##logoutUser()
      |> Js.Promise.then_(user =>
           user->Js.Nullable.toOption->Belt.Option.map(fromUserJs)
           |> Js.Promise.resolve
         ),
    requestPasswordRecovery: (~email) =>
      identity##requestPasswordRecovery(email),
    recoverAccount: (~token, ~remember=?, ()) =>
      identity##recoverAccount(token, Js.Nullable.fromOption(remember))
      |> Js.Promise.then_(user => fromUserJs(user) |> Js.Promise.resolve),
    updateUser: data =>
      identity##updateUser({"data": data})
      |> Js.Promise.then_(user =>
           user->Js.Nullable.toOption->Belt.Option.map(fromUserJs)
           |> Js.Promise.resolve
         ),
    getFreshJWT: () => identity##getFreshJWT(),
    url: identity##_url,
    loginProvider: provider =>
      identity##loginProvider(providerToString(provider)),
    acceptInviteExternalUrl: (~provider, ~token) =>
      identity##acceptInviteExternalUrl(providerToString(provider), token),
    settings: fromSettingsJs(identity##settings),
  };
};
