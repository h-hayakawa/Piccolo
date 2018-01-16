<!DOCTYPE html>
<html lang="ja" style="height:100%;">
  <head>
    <meta charset="utf-8">
    <title>NULLさんの実験室</title>
    <meta name="viewport" content="width=device-width,initial-scale=1" />
    <link rel="stylesheet" href="/css/bootstrap.css">
  </head>
  <body style="height:100%;">
    <nav class="navbar navbar-inverse" style="margin-bottom: 0px;">
    <div class="container-fluid">
        <div class="nav-header">
            <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false" aria-controls="navbar">
                <span class="sr-only">Toggle navigation</span>
                <span class="icon-bar"></span>
                <span class="icon-bar"></span>
                <span class="icon-bar"></span>
            </button>
            <a class="navbar-brand" href="/">top</a>
        </div><!-- nav-header -->
        <div id="navbar" class="navbar-collapse collapse in" area-expanded="true">
            <ul class="nav navbar-nav navbar-right">
                <li><a href="">お知らせ</a></li>
                <li><a href="">お問い合わせ</a></li>
                <li><a href="">このサイトについて</a></li>
            </ul>
            <form class="navbar-form navbar-right">
                <input type="text" class="form-control" placeholder="検索">
            </form>
        </div><!-- navbar -->
    </div><!-- container-fluid -->
  </nav>

  <div class="main-content" style="margin-left:20px;height:calc( 100% - 52px );width:calc( 100% - 35px );">
    <div class="row" style="height:100%;">
      <div class="col-md-3 sidebar">
        side
      </div><!--/.sidebar-->
      <div id="draw_window" class="col-md-9 content-area" style="border:solid 1px;height:100%; padding:0;">
        <!--
        <div id="image-box" class=tile><img /></div>
      -->
      </div><!--/.content-area-->
      <div id="pre_load_area" style="display:none;">
      </div>
      <input type="text" id="file_name_data" style="display:none;" value="<?php echo $imageName ?>">
    </div>
  </div><!--/.main-content-->
    
  


  </body>
  <script type="text/javascript" src="/js/jquery.1.12.4.min.js"></script>
  <script type="text/javascript" src="/js/bootstrap.min.js"></script>
  <script type="text/javascript" src="/js/note_edit.js"></script>
</html>