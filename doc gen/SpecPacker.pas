{$reference '0MarkDig\Markdig.dll'}
uses System.IO;

type
  HTML = static class
    // Справка расширений:
    // https://github.com/lunet-io/markdig/blob/master/src/Markdig.Tests/Specs/readme.md
    private static md_pipeline := Markdig.MarkdownPipelineBuilder.Create
      .UseEmphasisExtras    // Перечёркивание с ~~text~~ и т.п.
      .UseCustomContainers  // Блоки-спойлеры
      .UseGenericAttributes // Прямое указание атрибутов html элемента, надо для спойлеров
      .UsePipeTables(nil)   // Таблицы
    .Build;
    private static enc := new System.Text.UTF8Encoding(true);
    
    private static last_page_id: integer;
    
    public static procedure AddPage(sw: StreamWriter; path: string);
    begin
      last_page_id += 1;
      
      var page_name := System.IO.Path.GetFileName(path);
      if page_name.Contains('#') then page_name := page_name.Substring(page_name.IndexOf('#')+1);
      
      sw.WriteLine($'<div id="page-{last_page_id}" page_name="{page_name}" hidden=true>');
      
      if System.IO.File.Exists(path+'.css') then
      begin
        sw.WriteLine('<style>');
        sw.WriteLine(ReadAllText(path+'.css', enc).Trim);
        sw.WriteLine('</style>');
      end;
      
      sw.WriteLine(Markdig.Markdown.ToHtml(
        ReadAllText(path+'.md', enc),
        md_pipeline
      ).Trim);
      
      if System.IO.File.Exists(path+'.js') then
      begin
        sw.WriteLine('<script>');
        sw.WriteLine(ReadAllText(path+'.js', enc).Trim);
        sw.WriteLine('</script>');
      end;
      
      sw.WriteLine($'</div>');
    end;
    
    public static procedure AddFolder(sw: StreamWriter; path: string);
    begin
      var dir_name := System.IO.Path.GetFileName(path);
      if dir_name.Contains('#') then dir_name := dir_name.Substring(dir_name.IndexOf('#')+1);
      
      if &File.Exists(path+'\.md') then
      begin
        AddPage(sw, path+'\');
        sw.WriteLine($'<script>on_start_folder("{dir_name}", document.getElementById("page-{last_page_id}"))</script>');
      end else
        sw.WriteLine($'<script>on_start_folder("{dir_name}", null)</script>');
        
      foreach var dir in Directory.EnumerateDirectories(path) do
        AddFolder(sw, dir);
      
      foreach var fname in Directory.EnumerateFiles(path, '*.md') do
        if not fname.EndsWith('\.md') then
        begin
          AddPage(sw, fname.Remove(fname.LastIndexOf('.')));
          sw.WriteLine($'<script>on_page_added(document.getElementById("page-{last_page_id}"))</script>');
        end;
      
      sw.WriteLine($'<script>on_end_folder()</script>');
    end;
    
    public static procedure Pack(path, otp: string);
    begin
      var nick := otp.Remove(otp.LastIndexOf('.'));
      Writeln($'Packing spec "{nick}"');
      last_page_id := 0;
      
      var sw := new StreamWriter(otp, false, enc);
      sw.WriteLine('<html>');
      sw.WriteLine('<head>');
      sw.WriteLine('<meta charset="utf-8">');
      
      sw.WriteLine('<style>');
      sw.WriteLine(ReadAllText('0SpecContainer\.css', enc).Trim);
      sw.WriteLine('</style>');
      
      sw.WriteLine('</head>');
      sw.WriteLine('<body>');
      
      sw.WriteLine(Markdig.Markdown.ToHtml(
        ReadAllText('0SpecContainer\.md', enc),
        md_pipeline
      ).Trim);
      
      sw.WriteLine('<script>');
      sw.WriteLine(ReadAllText('0SpecContainer\.js', enc).Trim);
      sw.WriteLine('</script>');
      
      if System.IO.Directory.Exists(path) then AddFolder(sw, path);
      
      sw.WriteLine('</body>');
      sw.WriteLine('</html>');
      sw.Close;
      
      Writeln($'Done packing spec "{nick}"');
    end;
    
  end;
  
begin
  
  foreach var lang in |'ru', 'uk'| do
  begin
    
    HTML.Pack($'Doc source ({lang})', $'Все лабораторные работы ({lang}).html');
    foreach var dir in EnumerateDirectories($'Doc source ({lang})') do
      HTML.Pack(dir, $'{Path.GetFileName(dir)} ({lang}).html');
    
  end;
  
end.