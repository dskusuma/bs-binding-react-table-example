module ReactTable = {
  [@bs.module "react-table"] [@react.component]
  external make: (~data: 'data=?) => React.element = "default";
};

module Hooks = {
  /*
   * Decorator will generate a function that matching the name of
   * each field in the record and will return the fields type
   */
  [@bs.deriving accessors]
  type tableProps = {
    role: string,
    key: string,
  };

  [@bs.deriving accessors]
  type testAccessor = {nama: string};

  type headerGroupProps('column) = {
    getHeaderGroupProps: unit => tableProps,
    headers: array('column),
  };

  type cell = string;

  type column = {
    header: string,
    footer: string,
    accessor: string,
    columns: array(column),
    getHeaderProps: unit => tableProps,
    render: (. string) => React.element,
  };

  let make_column = (~header, ~footer, ~accessor: option(string), ~columns) => {
    "Header": header,
    "Footer": footer,
    "accessor": Js.Null.fromOption(accessor),
    "columns": columns,
  };

  type row('r) = {
    original: 'r,
    getRowProps: unit => string,
    cells: array(cell),
  };

  type renderProps('column, 'row) = {
    getTableProps: unit => tableProps,
    getTableBodyProps: unit => tableProps,
    headerGroups: array(headerGroupProps('column)),
    rows: array(row('row)),
    /* ASK: Buat apa? */
    prepareRow: row('row) => unit,
  };

  [@bs.module "react-table"]
  external useTable: 'a => renderProps(column, row('r)) = "useTable";
};

[@react.component]
let make = (~data as _) => {
  let table =
    Hooks.useTable({
      "columns": [|
        {
          "Header": "First Name",
          "Footer": "First Name",
          "accessor": "firstName",
        },
        {
          "Header": "Last Name",
          "Footer": "Last Name",
          "accessor": "lastName",
        },
      |],
      "data": [|
        {"firstName": "thomas", "lastName": "2"},
        {"firstName": "aude", "lastName": "23"},
        {"firstName": "damien", "lastName": "32"},
      |],
    });
  Js.log2("prepareRow", table.prepareRow);
  Js.log(table.headerGroups);
  Js.log(table.getTableProps());
  Js.log(table.getTableBodyProps());

  <table
    role={table.getTableProps() |> Hooks.role}
    key={table.getTableProps() |> Hooks.key}>
    <thead>
      {table.headerGroups
       ->Belt.Array.map(group => {
           let tableProps = group.getHeaderGroupProps();
           <tr role={tableProps.role} key={tableProps.key}>
             {group.headers
              ->Belt.Array.map(column => {
                  let thProps = column.getHeaderProps();
                  <th role={thProps.role} key={thProps.key}>
                    {column.render(. "Header")}
                  </th>;
                })
              ->React.array}
           </tr>;
         })
       ->React.array}
    </thead>
    <tbody role={table.getTableBodyProps() |> Hooks.role}>
      {table.rows
       ->Belt.Array.map(row => {
           Js.log2("row", row);
           Js.log2("row.getRowProps", row.getRowProps);
           Js.log2("row.original", row.original);

           /* <tr />; */
           React.null;
         })
       ->React.array}
    </tbody>
    <tfoot />
  </table>;
};
