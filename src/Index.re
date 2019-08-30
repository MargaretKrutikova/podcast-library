[%bs.raw {|require('./styles.css')|}];

let apolloClient = Graphql.makeApolloClient();

ReactDOMRe.renderToElementWithId(
  <AppCore.ElmishProvider>
    <ReasonApollo.Provider client=apolloClient>
      <ReasonApolloHooks.ApolloProvider client=apolloClient>
        <ReactNetlifyIdentity.IdentityContextProvider
          url="https://podcasts.margareta.dev/">
          <ThemeProvider> <Root /> </ThemeProvider>
        </ReactNetlifyIdentity.IdentityContextProvider>
      </ReasonApolloHooks.ApolloProvider>
    </ReasonApollo.Provider>
  </AppCore.ElmishProvider>,
  "root",
);
