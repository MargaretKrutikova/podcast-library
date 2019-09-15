type route =
  | MyLibrary
  | MyEpisodes(string)
  | Search(SearchQuery.t)
  | SearchEmpty
  | NotFound;

let urlToRoute = (url: ReasonReactRouter.url) =>
  switch (url.path) {
  | ["my-library", podcastId, "episodes"] => MyEpisodes(podcastId)
  | ["my-library"] => MyLibrary
  | []
  | ["search"] =>
    switch (url.search) {
    | "" => Search(SearchQuery.make())
    | search => Search(SearchQuery.Url.parse(search))
    }
  | _ => NotFound
  };

let useAppUrl = () => {
  let url = ReasonReactRouter.useUrl();
  urlToRoute(url);
};

let getUrlFromRoute = route => {
  switch (route) {
  | MyLibrary => "/my-library"
  | MyEpisodes(id) => {j|my-library/$id/my-episodes|j}
  | SearchEmpty => "/search"
  | Search(query) =>
    let params = SearchQuery.Url.stringify(query);
    Url.Params.buildUrl("/search", params);
  | _ => "/"
  };
};

let pushRoute = route => {
  route |> getUrlFromRoute |> ReasonReactRouter.replace;
};
