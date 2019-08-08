[%bs.raw {|require('./styles/identity-widget.css')|}];
[%bs.raw {|require('bootstrap/dist/css/bootstrap.css')|}];
[%bs.raw {|require('./styles.css')|}];

let apolloClient = Graphql.makeApolloClient();

ReactDOMRe.renderToElementWithId(
  <AppCore.ElmishProvider>
    <ReasonApollo.Provider client=apolloClient>
      <ReactNetlifyIdentityWidget.IdentityContextProvider
        url="https://podcasts.margareta.dev/">
        <Root />
      </ReactNetlifyIdentityWidget.IdentityContextProvider>
    </ReasonApollo.Provider>
  </AppCore.ElmishProvider>,
  "root",
);
