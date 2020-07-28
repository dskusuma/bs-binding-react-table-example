// Entry point

module Table = {
  let data = [|
    {"name": "thomas", "age": "25"},
    {"name": "aude", "age": "23"},
    {"name": "damien", "age": "32"},
  |];

  [@react.component]
  let make = () => {
    <ExampleReactTable data />;
  };
};

[@bs.val] external document: Js.t({..}) = "document";

// We're using raw DOM manipulations here, to avoid making you read
// ReasonReact when you might precisely be trying to learn it for the first
// time through the examples later.
let style = document##createElement("style");
document##head##appendChild(style);
style##innerHTML #= ExampleStyles.style;

let makeContainer = text => {
  let container = document##createElement("div");
  container##className #= "container";

  let title = document##createElement("div");
  title##className #= "containerTitle";
  title##innerText #= text;

  let content = document##createElement("div");
  content##className #= "containerContent";

  let () = container##appendChild(title);
  let () = container##appendChild(content);
  let () = document##body##appendChild(container);

  content;
};

let data = [|
  {"name": "thomas", "age": "25"},
  {"name": "aude", "age": "23"},
  {"name": "damien", "age": "32"},
|];

ReactDOMRe.render(<ExampleReactTable data />, makeContainer("Table Reason"));
