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
        <select id="mag_selector">
          <option value="0">8倍</option>
          <option value="1">4倍</option>
          <option value="2">2倍</option>
          <option value="3" selected="selected">1倍</option>
          <option value="4">1/2倍</option>
          <option value="5">1/4倍</option>
          <option value="6">1/8倍</option>
          <option value="7">1/12倍</option>
          <option value="8">1/16倍</option>
          <option value="9">1/20倍</option>
          <option value="10">1/24倍</option>
          <option value="11">1/28倍</option>
          <option value="12">1/32倍</option>
          <option value="13">1/36倍</option>
          <option value="14">1/40倍</option>
          <option value="15">1/44倍</option>
          <option value="16">1/48倍</option>
          <option value="17">1/52倍</option>
          <option value="18">1/56倍</option>
          <option value="19">1/60倍</option>
          <option value="20">1/64倍</option>
        </select>
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