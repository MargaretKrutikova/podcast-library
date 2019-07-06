[@bs.val] external encodeURIComponent: string => string = "";

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
    encodeURIComponent(key) ++ "=" ++ encodeURIComponent(value);

  let build = (url, params: array(param)) => {
    let params =
      params->Belt.Array.map(encodeParam) |> Js.Array.joinWith("&");

    switch (params) {
    | "" => url
    | _ => {j|$url?$params|j}
    };
  };
};
