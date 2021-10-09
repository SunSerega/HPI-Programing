## var start := 2;
var count := 10;

for var i := start+1 to count do
begin
  $'Project("{{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}}") = "ЛР {i}", "ЛР {i}\ЛР {i}.vcxproj", "{{0B18B908-2578-4D9F-90D5-14555177237F}}"'.Println;
  $'EndProject'.Println;
end;

var origin_folder := 'ЛР '+start;
var files :=
  EnumerateFiles(origin_folder, '*')
  .Select(fname->fname.SubString(origin_folder.Length))
.ToArray;

for var i := start+1 to count do
begin
  var new_folder := 'ЛР '+i;
  System.IO.Directory.CreateDirectory(new_folder);
  foreach var fname in files do
    WriteAllText(new_folder+fname.Replace(origin_folder, new_folder),
      ReadAllText(origin_folder+fname)
      .Replace(origin_folder, new_folder),
      new System.Text.UTF8Encoding(true)
    );
end;

