open BsReactstrap;

type itunesWebData =
  | Unknown
  | Fetching
  | Loaded(string)
  | Error;

let createItunesLinkHref = (podcastItunesId, itunesData) =>
  switch (itunesData) {
  | Loaded(episodeItunesId) =>
    ItunesLink.makeForEpisode(
      ~podcastItunesId=string_of_int(podcastItunesId),
      ~episodeItunesId,
    )
  | _ => "#"
  };

let openItunesLink = (~podcastItunesId, ~episodeItunesId) =>
  ItunesLink.makeForEpisode(
    ~podcastItunesId=string_of_int(podcastItunesId),
    ~episodeItunesId,
  )
  |> ItunesLink.setLocation;

[@react.component]
let make = (~podcastItunesId, ~episodeName) => {
  let dispatch = AppCore.useDispatch();
  let (itunesId, setItunesId) = React.useState(() => Unknown);

  let handleFetchError = () => {
    setItunesId(_ => Error);
    dispatch(
      ShowNotification({
        type_: Danger,
        text: "Couldn't retrieve itunes id :(",
      }),
    );
  };

  let handleFetchSuccess = episodeItunesId => {
    setItunesId(_ => Loaded(episodeItunesId));
    openItunesLink(~podcastItunesId, ~episodeItunesId);
  };

  let getItunesId = () => {
    setItunesId(_ => Fetching);
    ItunesEpisode.getItunesId(
      ~podcastItunesId=string_of_int(podcastItunesId),
      ~episodeName,
    )
    |> Js.Promise.(then_(id => handleFetchSuccess(id) |> resolve))
    |> Js.Promise.(catch(_ => handleFetchError() |> resolve));
  };

  let handleItunesLinkClick = () => {
    switch (itunesId) {
    | Loaded(episodeItunesId) =>
      openItunesLink(~podcastItunesId, ~episodeItunesId)
    | _ => getItunesId() |> ignore
    };
  };

  <NavLink
    onClick=handleItunesLinkClick
    disabled={itunesId == Fetching}
    href={createItunesLinkHref(podcastItunesId, itunesId)}>
    {ReasonReact.string("Open in itunes")}
  </NavLink>;
};
