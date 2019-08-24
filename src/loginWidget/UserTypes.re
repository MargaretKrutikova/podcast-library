type userMetadata = {. "full_name": Js.Nullable.t(string)};

module FormData = {
  type status =
    | NotAsked
    | Submitting
    | Error(string)
    | Success;

  type state = {
    email: string,
    password: string,
    fullName: string,
    status,
  };

  let initState = {email: "", password: "", fullName: "", status: NotAsked};
  type action =
    | SetEmail(string)
    | SetPassword(string)
    | SetFullName(string)
    | SubmitRequest
    | SubmitError(string)
    | SubmitSuccess;

  let reducer = (state, action) => {
    switch (action) {
    | SetEmail(email) => {...state, email}
    | SetPassword(password) => {...state, password}
    | SetFullName(fullName) => {...state, fullName}
    | SubmitRequest => {...state, status: Submitting}
    | SubmitError(msg) => {...state, status: Error(msg)}
    | SubmitSuccess => {...state, status: Success}
    };
  };
};
