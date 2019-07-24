module UpdateItunesId = [%graphql
  {|
  mutation($listennotesId: String!, $itunesId: String!) {
    update_episodes(
      where: {listennotesId: {_eq: $listennotesId}},
      _set: {itunesId: $itunesId}) {
        affected_rows
    }
  }
|}
];

module GetEpisodeItunesId = [%graphql
  {|
    query($podcastItunesId: String!, $episodeName: String!) {
      itunesEpisode (podcastId: $podcastItunesId, episodeName: $episodeName) {
        id
      }
    }
|}
];

let getItunesId = (~podcastItunesId, ~episodeName) =>
  GetEpisodeItunesId.make(~podcastItunesId, ~episodeName, ())
  |> Graphql.sendQuery
  |> Js.Promise.(then_(response => response##itunesEpisode##id |> resolve));

let updateItunesId = (~podcastItunesId, ~episodeId, ~episodeName) => {
  getItunesId(~podcastItunesId, ~episodeName)
  |> Js.Promise.(
       then_(itunesId =>
         switch (itunesId) {
         | None => resolve(0)
         | Some(id) =>
           UpdateItunesId.make(~listennotesId=episodeId, ~itunesId=id, ())
           |> Graphql.sendQuery
           |> then_(response =>
                switch (response##update_episodes) {
                | None => resolve(0)
                | Some(res) => resolve(res##affected_rows)
                }
              )
         }
       )
     );
};
