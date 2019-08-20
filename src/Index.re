[%bs.raw {|require('./styles/identity-widget.css')|}];
[%bs.raw {|require('bootstrap/dist/css/bootstrap.css')|}];
[%bs.raw {|require('./styles/styles.css')|}];

let apolloClient = Graphql.makeApolloClient();

ReactDOMRe.renderToElementWithId(
  <AppCore.ElmishProvider>
    <ReasonApollo.Provider client=apolloClient>
      <ReasonApolloHooks.ApolloProvider client=apolloClient>
        <ReactNetlifyIdentityWidget.IdentityContextProvider
          url="https://podcasts.margareta.dev/">
          <ThemeProvider> <Root /> </ThemeProvider>
        </ReactNetlifyIdentityWidget.IdentityContextProvider>
      </ReasonApolloHooks.ApolloProvider>
    </ReasonApollo.Provider>
  </AppCore.ElmishProvider>,
  "root",
);
