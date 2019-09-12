type t = {
  query: string,
  content: ContentType.t,
};

let valueFromDict = (key, dict) => {
  switch (Js.Dict.get(dict, key)) {
  | Some(Url.Query.Single(search)) => search
  | Some(Multiple(array)) when Js.Array.length(array) > 0 =>
    Js.Array.unsafe_get(array, 0)
  | _ => ""
  };
};

let parse = queryString => {
  let dict = Url.Query.parse(queryString);
  let query = valueFromDict("q", dict);
  let content = valueFromDict("content", dict) |> ContentType.fromString;

  {query, content};
};

let stringify = query => {
  [||]
  |> Url.Params.add("q", query.query)
  |> Url.Params.add("content", query.content |> ContentType.toString);
};
