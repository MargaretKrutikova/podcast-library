[@bs.val] external grapqhlUrl: string = "process.env.REACT_APP_GRAPHQL_URL";
let fragmentsData = [%bs.raw {|require('../../fragmentTypes.json')|}];

exception Graphql_error(string);

let makeApolloClient = _ => {
  // Create an InMemoryCache
  let fragmentMatcher =
    ApolloInMemoryCache.createIntrospectionFragmentMatcher(
      ~data=fragmentsData,
    );
  let inMemoryCache =
    ApolloInMemoryCache.createInMemoryCache(~fragmentMatcher, ());

  // Create an HTTP Link
  let httpLink = ApolloLinks.createHttpLink(~uri=grapqhlUrl, ());

  // return apollo client instance
  ReasonApollo.createApolloClient(~link=httpLink, ~cache=inMemoryCache, ());
};

let sendQuery = q =>
  Axios.postDatac(
    grapqhlUrl,
    {"query": Js.Json.string(q##query), "variables": q##variables},
    Axios.makeConfig(
      ~withCredentials=true,
      ~headers=Axios.Headers.fromObj({"content-type": "application/json"}),
      (),
    ),
  )
  |> Js.Promise.then_(resp =>
       resp##data->Js.Dict.unsafeGet("data") |> q##parse |> Js.Promise.resolve
     )
  |> Js.Promise.catch(error => {
       let error = error |> Utils.promiseErrorToJsObj;
       Js.log(error);

       Js.Promise.reject(
         Graphql_error("Request failed: " ++ error##response),
       );
     });
