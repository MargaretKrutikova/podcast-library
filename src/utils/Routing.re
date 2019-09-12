type route =
  | MyLibrary
  | MyEpisodes(string)
  | Search(SearchQs.t)
  | SearchEmpty
  | NotFound;

let useAppUrl = () => {
  let url = ReasonReactRouter.useUrl();

  switch (url.path) {
  | ["my-library", podcastId, "episodes"] => MyEpisodes(podcastId)
  | ["my-library"] => MyLibrary
  | []
  | ["search"] =>
    switch (url.search) {
    | "" => Search({query: "", content: Episode})
    | search => Search(SearchQs.parse(search))
    }
  | _ => NotFound
  };
};

let getUrlFromRoute = route => {
  switch (route) {
  | MyLibrary => "/my-library"
  | MyEpisodes(id) => {j|my-library/$id/my-episodes|j}
  | SearchEmpty => "/search"
  | Search(query) =>
    let params = SearchQs.stringify(query);
    Url.Params.buildUrl("/search", params);
  | _ => "/"
  };
};

let pushRoute = route => {
  route |> getUrlFromRoute |> ReasonReactRouter.replace;
};
