[@bs.val]
external listenApiKey: string = "process.env.REACT_APP_LISTEN_API_KEY";
external promiseErrorToJsObj: Js.Promise.error => Js.t('a) = "%identity";

let baseURL = "https://listen-api.listennotes.com/api/v2";

let headers = Axios.Headers.fromObj({"X-ListenAPI-Key": listenApiKey});

let axiosInstance =
  Axios.(Instance.create(makeConfig(~baseURL, ~headers, ())));

let search =
    (searchQuery: SearchQuery.t)
    : Js.Promise.t(Belt.Result.t(SearchResult.t, unit)) =>
  Js.Promise.(
    Axios.Instance.get(
      axiosInstance,
      UrlBuilder.Params.build("/search", SearchQuery.toParams(searchQuery)),
    )
    |> then_(resp => {
         let data = SearchResult.decode(resp##data);
         resolve(Belt.Result.Ok(data));
       })
    |> catch(error => {
         let error = error |> promiseErrorToJsObj;
         Js.log(error);
         Belt.Result.Error(error##response) |> resolve;
       })
  );
