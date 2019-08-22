type searchQuery = {query: option(string)};

type route =
  | MyLibrary
  | MyEpisodes(string)
  | Search(searchQuery)
  | NotFound;

let searchQueryFromDict = dict => {
  switch (Js.Dict.get(dict, "q")) {
  | Some(Url.Query.Single(s)) => {query: Some(s)}
  | Some(Multiple(array)) when Js.Array.length(array) > 0 => {
      query: Some(Js.Array.unsafe_get(array, 0)),
    }
  | _ => {query: None}
  };
};

let useAppUrl = () => {
  let url = ReasonReactRouter.useUrl();

  Js.log(url);

  switch (url.path) {
  | ["my-library", podcastId, "episodes"] => MyEpisodes(podcastId)
  | ["my-library"] => MyLibrary
  | []
  | ["search"] =>
    switch (url.search) {
    | "" => Search({query: None})
    | search =>
      let dict = Url.Query.parse(search);
      Js.log(dict);

      Search(searchQueryFromDict(dict));
    }
  | _ => NotFound
  };
};

let getUrlFromRoute = route => {
  switch (route) {
  | MyLibrary => "/my-library"
  | MyEpisodes(id) => {j|my-library/$id/my-episodes|j}
  | Search(query) =>
    let params = [||] |> Url.Params.addOption("q", query.query);
    Url.Params.buildUrl("/search", params);
  | _ => "/"
  };
};

let pushRoute = route => {
  route |> getUrlFromRoute |> ReasonReactRouter.replace;
};
