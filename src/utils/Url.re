module type Query = {
  type queryItem;
  let parse: string => Js.Dict.t(queryItem);
};

// Belt_MapString
module Query = {
  type queryItem =
    | Single(string)
    | Multiple(array(string));

  let addToDict = (dict, keyValue) => {
    switch (Js.String.split("=", keyValue)) {
    | [|"", ""|] => dict
    | [|key, encodedValue|] =>
      let value = Js.Global.decodeURIComponent(encodedValue);
      switch (Js.Dict.get(dict, key)) {
      | None => Js.Dict.set(dict, key, Single(value))
      | Some(Single(singleVal)) =>
        Js.Dict.set(dict, key, Multiple([|singleVal, value|]))
      | Some(Multiple(multiVal)) =>
        let dictVal = Js.Array.concat(multiVal, [|value|]);
        Js.Dict.set(dict, key, Multiple(dictVal));
      };
      dict;
    | _ => dict
    };
  };

  let parse = (query: string) => {
    let queryObj = Js.Dict.empty();

    let pairs = Js.String.split("&", query);
    Array.fold_left(addToDict, queryObj, pairs);
  };
};

module type Params = {
  type param;
  let add: (string, string) => array(param);
  let addOption: (string, option(string)) => array(param);
  let buildUrl: (string, array(param)) => string;
};

module Params = {
  type param = (string, string);

  let add = (key, value, params: array(param)) =>
    Belt.Array.concat(params, [|(key, value)|]);

  let addOption = (key, valueOption, params: array(param)) => {
    switch (valueOption) {
    | None => params
    | Some(value) => add(key, value, params)
    };
  };

  let encodeParam = ((key, value)) =>
    Js.Global.encodeURIComponent(key)
    ++ "="
    ++ Js.Global.encodeURIComponent(value);

  let buildUrl = (path, params: array(param)) => {
    let params =
      params->Belt.Array.map(encodeParam) |> Js.Array.joinWith("&");

    switch (params) {
    | "" => path
    | _ => {j|$path?$params|j}
    };
  };
};
