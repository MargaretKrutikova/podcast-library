open BsReactstrap;
open SearchResult;

let str = ReasonReact.string;

module SearchEpisodesQuery =
  ReasonApollo.CreateQuery(EpisodeSearch.SearchEpisodes);

module SearchPodcastsQuery =
  ReasonApollo.CreateQuery(PodcastSearch.SearchPodcasts);

module GetSavedIdsQuery =
  ReasonApollo.CreateQuery(MyLibrary.GetMyLibrarySavedIds);

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
  let searchModel = AppCore.useSelector(s => s.search);

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

  <GetSavedIdsQuery variables=MyLibrary.makeGetSavedIdsQuery()##variables>
    ...{savedIdsResult =>
      <>
        <h1> {str("Search library")} </h1>
        <SearchQueryView />
        {switch (searchModel.searchType) {
         | Episode =>
           <SearchEpisodesQuery
             variables={getEpisodeSearchVariables()}
             skip=shouldSkipSearch
             notifyOnNetworkStatusChange=true>
             ...{({result, fetchMore, networkStatus}) =>
               <SearchResultView
                 result
                 renderData={data =>
                   <div>
                     {data##searchEpisodes.results
                      ->Belt.Array.map(episode =>
                          <EpisodeView
                            key={episode.listennotesId}
                            episode
                            isSaved={MyLibrary.isEpisodeSaved(
                              savedIdsResult.result,
                              episode.listennotesId,
                            )}
                          />
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
                       isFetching={
                         networkStatus === ReasonApolloTypes.FetchMore
                       }
                     />
                   </div>
                 }
               />
             }
           </SearchEpisodesQuery>
         | Podcast =>
           <SearchPodcastsQuery
             variables={getPodcastSearchVariables()}
             skip=shouldSkipSearch
             notifyOnNetworkStatusChange=true>
             ...{({result, fetchMore, networkStatus}) =>
               <SearchResultView
                 result
                 renderData={data =>
                   <div>
                     {data##searchPodcasts.results
                      ->Belt.Array.map(podcast =>
                          <PodcastView
                            key={podcast.listennotesId}
                            podcast
                            isSaved={MyLibrary.isPodcastSaved(
                              savedIdsResult.result,
                              podcast.listennotesId,
                            )}
                          />
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
                       isFetching={
                         networkStatus === ReasonApolloTypes.FetchMore
                       }
                     />
                   </div>
                 }
               />
             }
           </SearchPodcastsQuery>
         }}
      </>
    }
  </GetSavedIdsQuery>;
};
