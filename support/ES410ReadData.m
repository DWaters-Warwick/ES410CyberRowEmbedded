function table = ES410ReadData(filename)

    filepath = fullfile(pwd , filename);
    RawTable = readtable(filepath,'Delimiter',{',', ':'});
    tableData = RawTable(:,2:2:end);
    tableNames = RawTable(1,1:2:end);
    tableData.Properties.VariableNames = table2cell(tableNames);

    table = tableData;
end

