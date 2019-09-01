let setWindowLocation: string => unit = [%bs.raw
  {| function (href) { window.location.href = href; } |}
];

/** itunes */
let itunesBaseUrl = "https://podcasts.apple.com/us/podcast/";

let makePodcastItunesUrl = podcastItunesId =>
  itunesBaseUrl ++ "id" ++ podcastItunesId;

let makeEpisodeItunesUrl = (~podcastItunesId, ~episodeItunesId) =>
  itunesBaseUrl
  ++ episodeItunesId
  ++ "/id"
  ++ podcastItunesId
  ++ "?i="
  ++ episodeItunesId;

/** UI */
let maxDescrptionTextLength = 200;

let truncateDescription = description =>
  String.length(description) < maxDescrptionTextLength
    ? description : description ++ "...";

/** reason apollo */

let toWriteQueryOptions = (~query, ~data) => {
  "query": ApolloClient.gql(. query##query),
  "variables": Js.Nullable.fromOption(Some(query##variables)),
  "data": data,
};

let hasMutationData = (result: ReasonApolloTypes.mutationResponse('a)) => {
  switch (result) {
  | Data(_) => true
  | _ => false
  };
};

/** api data */
let toApiGenres = (arr: array(int)) => Js.Array.joinWith(",", arr);

[@bs.val] external scrollTop: float = "document.scrollingElement.scrollTop";

type elementBox = {
  top: int,
  left: int,
  right: int,
  width: int,
  height: int,
};
let getElRect = element => {
  let element = ReactDOMRe.domElementToObj(element);
  let box = element##getBoundingClientRect();
  {
    top: box##top,
    left: box##left,
    right: box##right,
    width: box##width,
    height: box##height,
  };
};

let getRectFromRef = elRef => {
  elRef->React.Ref.current->Js.Nullable.toOption->Belt.Option.map(getElRect);
};

/** general */
let fromBigInt = value =>
  value->Js.Json.decodeNumber->Belt.Option.mapWithDefault(0, int_of_float);

let makePositive = number => number < 0 ? 0 : number;

/** inputs */
let getInputValue = (e): string => ReactEvent.Form.target(e)##value;

type keyPressed =
  | Enter
  | Escape
  | Other;

let onKeyDown = event => {
  let key = ReactEvent.Keyboard.which(event);
  switch (key) {
  | 27 => Escape
  | 13 => Enter
  | _ => Other
  };
};

/** material ui */
external styleToString: ReactDOMRe.Style.t => string = "%identity";

let px_of_int = x => x->string_of_int ++ "px";

let spacingPx = (ind, theme) =>
  theme->MaterialUi.Theme.Theme.spacingGet(ind);

let spacing = (ind, theme) =>
  theme->MaterialUi.Theme.Theme.spacingGet(ind)->px_of_int;

let spacingTransform = (ind, transform, theme) =>
  theme->MaterialUi.Theme.Theme.spacingGet(ind)->transform->px_of_int;

let getPalette = theme => theme->MaterialUi.Theme.Theme.paletteGet;

let getPrimaryColor = theme =>
  getPalette(theme)->MaterialUi.Theme.Palette.primaryGet;

let getSecondaryColor = theme =>
  getPalette(theme)->MaterialUi.Theme.Palette.secondaryGet;

let fromMuiColor = Js.String.replace("#", "");

let getBreakpoint = (breakpoint, theme) => {
  let muiBreakpoint =
    switch (breakpoint) {
    | `XS => "xs"->MaterialUi.ThemeHelpers.stringToBreakpoint
    | `SM => "sm"->MaterialUi.ThemeHelpers.stringToBreakpoint
    | `MD => "md"->MaterialUi.ThemeHelpers.stringToBreakpoint
    | `LG => "lg"->MaterialUi.ThemeHelpers.stringToBreakpoint
    | `XL => "xl"->MaterialUi.ThemeHelpers.stringToBreakpoint
    | `Int(x) => x->MaterialUi.ThemeHelpers.intToBreakpoint
    };

  theme
  ->MaterialUi.ThemeHelpers.Theme.breakpointsGet
  ->MaterialUi.ThemeHelpers.Breakpoints.upGet
  ->MaterialUi.ThemeHelpers.jsonToBreakpointFunc(muiBreakpoint)
  |> Js.String.replace("@media", "");
};
