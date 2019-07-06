[@react.component]
let make = () => {
  let (results, setResults) = React.useState(() => None);

  React.useEffect0(() => {
    let query = {
      ...SearchQuery.defaultValue(),
      searchTerm: "javascript",
      searchFields: Some([|Author|]),
    };
    Js.Promise.(
      ListenNotesApi.search(query)
      |> then_(result =>
           switch (result) {
           | Belt.Result.Ok(searchResult) =>
             setResults(_ => Some(searchResult)) |> resolve
           | Belt.Result.Error () => resolve()
           }
         )
    )
    |> ignore;

    None;
  });

  Js.log(results);

  <div>
    <button> {ReasonReact.string("Click")} </button>
    <div>
      {switch (results) {
       | None => <span> {ReasonReact.string("Nothing yet")} </span>
       | Some(res) =>
         res.results
         ->Belt.Array.map(podcast =>
             <div key={podcast.id}> {ReasonReact.string(podcast.title)} </div>
           )
         |> ReasonReact.array
       }}
    </div>
  </div>;
};
