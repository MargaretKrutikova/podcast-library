[%bs.raw {|require('bootstrap/dist/css/bootstrap.css')|}];
[%bs.raw {|require('./styles.css')|}];

ReactDOMRe.renderToElementWithId(
  <AppCore.ElmishProvider> <Root /> </AppCore.ElmishProvider>,
  "root",
);
