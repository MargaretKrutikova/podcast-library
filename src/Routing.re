type searchQuery = {query: string};

type route =
  | MyLibrary
  | MyEpisodes(string)
  | Search(searchQuery)
  | NotFound;

let searchQueryFromDict = dict => {
  switch (Js.Dict.get(dict, "q")) {
  | Some(Url.Query.Single(s)) => {query: s}
  | Some(Multiple(array)) when Js.Array.length(array) > 0 => {
      query: Js.Array.unsafe_get(array, 0),
    }
  | _ => {query: ""}
  };
};

let useAppUrl = () => {
  let url = ReasonReactRouter.useUrl();

  switch (url.path) {
  | ["my-library", podcastId, "episodes"] => MyEpisodes(podcastId)
  | ["my-library"] => MyLibrary
  | []
  | ["search"] =>
    switch (url.search) {
    | "" => Search({query: ""})
    | search =>
      let dict = Url.Query.parse(search);
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
    let params = [||] |> Url.Params.add("q", query.query);
    Url.Params.buildUrl("/search", params);
  | _ => "/"
  };
};

let pushRoute = route => {
  route |> getUrlFromRoute |> ReasonReactRouter.replace;
};
