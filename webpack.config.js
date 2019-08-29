const path = require("path")
const HtmlWebpackPlugin = require("html-webpack-plugin")
const outputDir = path.join(__dirname, "build/")

const isProd = process.env.NODE_ENV === "production"
const Dotenv = require("dotenv-webpack")

module.exports = {
  entry: "./src/Index.bs.js",
  mode: isProd ? "production" : "development",
  output: {
    path: outputDir,
    filename: "Index.js"
  },
  devtool: "source-map",
  plugins: [
    new HtmlWebpackPlugin({
      template: "src/index.html",
      inject: false
    }),
    new Dotenv({ systemvars: true })
  ],
  devServer: {
    compress: true,
    contentBase: outputDir,
    port: process.env.PORT || 8000,
    historyApiFallback: true
  },
  module: {
    rules: [
      {
        test: /\.css$/,
        use: ["style-loader", "css-loader"]
      }
    ]
  }
}
