open ReactNetlifyIdentityWidget;

type user =
  | LoggedIn(string)
  | Anonymous;

let getUserMetaData = (identity: Js.Nullable.t(identityContextValue)) =>
  identity
  ->Js.Nullable.toOption
  ->Belt.Option.flatMap(v => v##user->Js.Nullable.toOption)
  ->Belt.Option.flatMap(v => v##user_metadata->Js.Nullable.toOption);

let getUserId = (identity: Js.Nullable.t(identityContextValue)) =>
  identity
  ->Js.Nullable.toOption
  ->Belt.Option.flatMap(v => v##user->Js.Nullable.toOption)
  ->Belt.Option.flatMap(v => Some(v##id));

let isLoggedIn = identity =>
  identity
  ->Js.Nullable.toOption
  ->Belt.Option.flatMap(v => v##isLoggedIn->Js.Nullable.toOption)
  ->Belt.Option.getWithDefault(false);

let useLoggedInUser = (): user => {
  let user = useIdentityContext();

  if (!isLoggedIn(user)) {
    Anonymous;
  } else {
    getUserId(user)
    ->Belt.Option.mapWithDefault(Anonymous, id => LoggedIn(id));
  };
};
