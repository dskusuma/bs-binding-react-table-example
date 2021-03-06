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

  type headerGroupProps('headers) = {
    getHeaderGroupProps: unit => tableProps,
    headers: array('headers),
  };

  type footerGroupProps('footers) = {
    getFooterGroupProps: unit => tableProps,
    headers: array('footers),
  };

  type cell = {
    getCellProps: unit => tableProps,
    render: (. string) => React.element,
  };

  type column = {
    header: string,
    footer: string,
    accessor: string,
    columns: array(column),
    getHeaderProps: unit => tableProps,
    getFooterProps: unit => tableProps,
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
    getRowProps: unit => tableProps,
    cells: array(cell),
  };

  type renderProps('column, 'row) = {
    getTableProps: unit => tableProps,
    getTableBodyProps: unit => tableProps,
    headerGroups: array(headerGroupProps('column)),
    footerGroups: array(footerGroupProps('column)),
    rows: array(row('row)),
    /* ASK: Buat apa? */
    prepareRow: row('row) => unit,
  };

  [@bs.module "react-table"]
  external useTable: 'a => renderProps(column, row('r)) = "useTable";
};

[@react.component]
let make = (~data as _) => {
  /* Example data */
  let table =
    Hooks.useTable({
      "columns": [|
        {
          "Header": "First Name",
          "Footer": "Footer First Name",
          "accessor": "firstName",
        },
        {
          "Header": "Last Name",
          "Footer": "Footer Last Name",
          "accessor": "lastName",
        },
        {"Header": "Age", "Footer": "Footer Age", "accessor": "age"},
      |],
      "data": [|
        {"firstName": "thomas", "lastName": "2"},
        {"firstName": "aude", "lastName": "23"},
        {"firstName": "damien", "lastName": "32"},
      |],
    });

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
           table.prepareRow(row);
           let rowProps = row.getRowProps();
           <tr role={rowProps.role} key={rowProps.key}>
             {Belt.Array.map(
                row.cells,
                cell => {
                  let cellProps = cell.getCellProps();
                  <td key={cellProps.key} role={cellProps.role}>
                    {cell.render(. "Cell")}
                  </td>;
                },
              )
              ->React.array}
           </tr>;
         })
       ->React.array}
    </tbody>
    <tfoot>
      {table.footerGroups
       ->Belt.Array.map(group => {
           let footerRowProps = group.getFooterGroupProps();
           <tr key={footerRowProps.key} role={footerRowProps.role}>
             {group.headers
              ->Belt.Array.map(column => {
                  let tdProps = column.getFooterProps();
                  <td role={tdProps.role} key={tdProps.key}>
                    {column.render(. "Footer")}
                  </td>;
                })
              ->React.array}
           </tr>;
         })
       ->React.array}
    </tfoot>
  </table>;
};
