switch ReactDOM.querySelector("#root") {
| Some(el) => ReactDOM.render(<App />, el)
| None => ()
}
