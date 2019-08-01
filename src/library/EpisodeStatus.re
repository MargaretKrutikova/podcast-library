type t =
  | NotListened
  | Started
  | Listened;

let decode = s =>
  switch (s) {
  | "NotListened" => NotListened
  | "Started" => Started
  | "Listened" => Listened
  | _ => NotListened
  };

let encode = s =>
  switch (s) {
  | NotListened => "NotListened"
  | Started => "Started"
  | Listened => "Listened"
  };
