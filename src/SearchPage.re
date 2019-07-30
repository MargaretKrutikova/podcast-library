open BsReactstrap;
open SearchResult;

let str = ReasonReact.string;

module SearchEpisodesQuery =
  ReasonApollo.CreateQuery(EpisodeSearch.SearchEpisodes);

module SearchPodcastsQuery =
  ReasonApollo.CreateQuery(PodcastSearch.SearchPodcasts);

module LoadMoreButton = {
  [@react.component]
  let make = (~nextOffset, ~total, ~isFetching, ~onClick) => {
    let hasMore = nextOffset < total;

    hasMore
      ? <div className="load-more-button-container">
          <Button color="info" onClick disabled=isFetching>
            {str("Fetch more")}
          </Button>
        </div>
      : ReasonReact.null;
  };
};

module SearchResultView = {
  [@react.component]
  let make =
      (
        ~result: ReasonApolloTypes.queryResponse('a),
        ~renderData: 'a => React.element,
      ) => {
    switch (result) {
    | Loading => <div> {ReasonReact.string("Loading")} </div>
    | Error(error) => <div> {ReasonReact.string(error##message)} </div>
    | Data(response) => renderData(response)
    };
  };
};

[@react.component]
let make = () => {
  let dispatch = AppCore.useDispatch();
  let searchModel = AppCore.useSelector(s => s.search);

  React.useEffect0(() => {
    dispatch(FetchLibraryIds);
    None;
  });

  let shouldSkipSearch = String.trim(searchModel.baseQuery.searchTerm) === "";

  /** episode search */
  let getEpisodeSearchVariables = (~nextOffset=0, ()): Js.Json.t =>
    EpisodeSearch.makeSearchQuery(
      searchModel.baseQuery,
      nextOffset,
      searchModel.episodeQuery,
    )##variables;

  let fetchMoreEpisodeResults = (~fetchMore, ~nextOffset): Js.Promise.t(unit) =>
    fetchMore(
      ~variables=?Some(getEpisodeSearchVariables(~nextOffset, ())),
      ~updateQuery=EpisodeSearch.fetchMoreUpdateQuery,
      (),
    );

  /** podcast search */
  let getPodcastSearchVariables = (~nextOffset=0, ()): Js.Json.t =>
    PodcastSearch.makeSearchQuery(searchModel.baseQuery, nextOffset)##variables;

  let fetchMorePodcastResults = (~fetchMore, ~nextOffset): Js.Promise.t(unit) =>
    fetchMore(
      ~variables=?Some(getPodcastSearchVariables(~nextOffset, ())),
      ~updateQuery=PodcastSearch.fetchMoreUpdateQuery,
      (),
    );

  <>
    <h1> {str("Search library")} </h1>
    <SearchQueryView hasSearchResult=true /> // TODO: fix this
    {switch (searchModel.searchType) {
     | Episode =>
       <SearchEpisodesQuery
         variables={getEpisodeSearchVariables()} skip=shouldSkipSearch>
         ...{({result, fetchMore}) =>
           <SearchResultView
             result
             renderData={data =>
               <div>
                 {data##searchEpisodes.results
                  ->Belt.Array.map(episode =>
                      <EpisodeView key={episode.listennotesId} episode />
                    )
                  |> ReasonReact.array}
                 <LoadMoreButton
                   nextOffset={data##searchEpisodes.nextOffset}
                   total={data##searchEpisodes.total}
                   onClick={_ =>
                     fetchMoreEpisodeResults(
                       ~fetchMore,
                       ~nextOffset=data##searchEpisodes.nextOffset,
                     )
                   }
                   isFetching=false // TODO: fix this
                 />
               </div>
             }
           />
         }
       </SearchEpisodesQuery>
     | Podcast =>
       <SearchPodcastsQuery
         variables={getPodcastSearchVariables()} skip=shouldSkipSearch>
         ...{({result, fetchMore}) =>
           <SearchResultView
             result
             renderData={data =>
               <div>
                 {data##searchPodcasts.results
                  ->Belt.Array.map(podcast =>
                      <PodcastView key={podcast.listennotesId} podcast />
                    )
                  |> ReasonReact.array}
                 <LoadMoreButton
                   nextOffset={data##searchPodcasts.nextOffset}
                   total={data##searchPodcasts.total}
                   onClick={_ =>
                     fetchMorePodcastResults(
                       ~fetchMore,
                       ~nextOffset=data##searchPodcasts.nextOffset,
                     )
                   }
                   isFetching=false // TODO: fix this
                 />
               </div>
             }
           />
         }
       </SearchPodcastsQuery>
     }}
  </>;
};
