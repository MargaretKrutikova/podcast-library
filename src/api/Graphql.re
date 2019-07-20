external promiseErrorToJsObj: Js.Promise.error => Js.t('a) = "%identity";

exception Graphql_error(string);

let apiLink = "http://localhost:8080/v1/graphql";

let sendQuery = q =>
  Axios.postDatac(
    apiLink,
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
       let error = error |> promiseErrorToJsObj;
       Js.log(error);

       Js.Promise.reject(
         Graphql_error("Request failed: " ++ error##response),
       );
     });
