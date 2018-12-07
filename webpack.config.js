const path = require("path");

module.exports = {
  entry: path.join(__dirname, "example/app/Example.bs.js"),
  mode: "development",
  output: {
    path: path.resolve(__dirname, "example/build"),
    publicPath: "/public/",
    filename: "Index.js"
  },
  devServer: {
    open: true,
    contentBase: path.resolve(__dirname, "example/public")
  },
  module: {
    rules: [
      {
        test: /\.svg$/,
        loader: "url-loader"
      }
    ]
  }
};
