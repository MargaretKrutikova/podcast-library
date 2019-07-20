open LibraryEpisode;

module GetMyLibraryPartial = [%graphql
  {|
  query($user_id: String!) {
    my_episodes @bsRecord (where: {user_id: {_eq: $user_id}}) {
      status @bsDecoder(fn: "statusDecoder")
      episode @bsRecord{
        listennotes_id
      }
    }
  }
  |}
];

let getPartial = () => {
  GetMyLibraryPartial.make(~user_id="margaretkru", ()) |> Graphql.sendQuery;
};
