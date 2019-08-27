type userMetaData = {fullName: string};
type userMetaDataJs = {. "full_name": string};

module Context =
  ReactNetlifyIdentity.Make({
    type userData = userMetaData;
    type userDataJs = userMetaDataJs;
    let convertFromJs = jsObj => {fullName: jsObj##full_name};
    let convertToJs = obj => {"full_name": obj.fullName};
  });

type appUser =
  | LoggedIn(string)
  | Anonymous;

let getLoggedInUser =
    (identity: ReactNetlifyIdentity.reactNetlifyIdentityApi(userMetaData))
    : appUser =>
  if (!identity.isLoggedIn) {
    Anonymous;
  } else {
    identity.user
    ->Belt.Option.mapWithDefault(Anonymous, user => LoggedIn(user.id));
  };
