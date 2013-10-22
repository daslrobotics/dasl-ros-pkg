

<!DOCTYPE html>
<html>
  <head prefix="og: http://ogp.me/ns# fb: http://ogp.me/ns/fb# githubog: http://ogp.me/ns/fb/githubog#">
    <meta charset='utf-8'>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <title>sr-taco/sr_visual_servoing/include/sr_visual_servoing/utils.hpp at F_moveit · shadow-robot/sr-taco</title>
    <link rel="search" type="application/opensearchdescription+xml" href="/opensearch.xml" title="GitHub" />
    <link rel="fluid-icon" href="https://github.com/fluidicon.png" title="GitHub" />
    <link rel="apple-touch-icon" sizes="57x57" href="/apple-touch-icon-114.png" />
    <link rel="apple-touch-icon" sizes="114x114" href="/apple-touch-icon-114.png" />
    <link rel="apple-touch-icon" sizes="72x72" href="/apple-touch-icon-144.png" />
    <link rel="apple-touch-icon" sizes="144x144" href="/apple-touch-icon-144.png" />
    <link rel="logo" type="image/svg" href="https://github-media-downloads.s3.amazonaws.com/github-logo.svg" />
    <meta property="og:image" content="https://a248.e.akamai.net/assets.github.com/images/modules/logos_page/Octocat.png">
    <link rel="assets" href="https://a248.e.akamai.net/assets.github.com/">
    <link rel="xhr-socket" href="/_sockets" />
    
    


    <meta name="msapplication-TileImage" content="/windows-tile.png" />
    <meta name="msapplication-TileColor" content="#ffffff" />
    <meta name="selected-link" value="repo_source" data-pjax-transient />
    <meta content="collector.githubapp.com" name="octolytics-host" /><meta content="github" name="octolytics-app-id" /><meta content="1252781" name="octolytics-actor-id" /><meta content="christopherKorpela" name="octolytics-actor-login" /><meta content="bfadff63f0b1ca71615d51ee843c81c9ce4a3efac5e868a20580e331b5b33b8f" name="octolytics-actor-hash" />

    
    
    <link rel="icon" type="image/x-icon" href="/favicon.ico" />

    <meta content="authenticity_token" name="csrf-param" />
<meta content="hCZAZTvQyHpLdZSvXSYgNXDR1l7Fv35gtv33kB091TQ=" name="csrf-token" />

    <link href="https://a248.e.akamai.net/assets.github.com/assets/github-05d9b1587ff8e26708f128a98aa2f34fbcd41f56.css" media="all" rel="stylesheet" type="text/css" />
    <link href="https://a248.e.akamai.net/assets.github.com/assets/github2-1ba35c8998f86af4160cf25c996d0e27ee06f105.css" media="all" rel="stylesheet" type="text/css" />
    


      <script src="https://a248.e.akamai.net/assets.github.com/assets/frameworks-1f72571b966545f4e27481a3b0ebbeeed4f2f139.js" type="text/javascript"></script>
      <script src="https://a248.e.akamai.net/assets.github.com/assets/github-7c3009c3f469a703105b0afd46d305ba722ddb1a.js" type="text/javascript"></script>
      
      <meta http-equiv="x-pjax-version" content="450c555b842981ec325f654418916e79">

        <link data-pjax-transient rel='permalink' href='/shadow-robot/sr-taco/blob/b4f4908bba1f258e3e60f3f4754841d865b343cf/sr_visual_servoing/include/sr_visual_servoing/utils.hpp'>

  <meta property="og:title" content="sr-taco"/>
  <meta property="og:type" content="githubog:gitrepository"/>
  <meta property="og:url" content="https://github.com/shadow-robot/sr-taco"/>
  <meta property="og:image" content="https://a248.e.akamai.net/assets.github.com/images/gravatars/gravatar-user-420.png"/>
  <meta property="og:site_name" content="GitHub"/>
  <meta property="og:description" content="sr-taco - Shadow Robot&#39;s demoes using the TACO sensor."/>

  <meta name="description" content="sr-taco - Shadow Robot&#39;s demoes using the TACO sensor." />

  <meta content="3306493" name="octolytics-dimension-user_id" /><meta content="shadow-robot" name="octolytics-dimension-user_login" /><meta content="7731364" name="octolytics-dimension-repository_id" /><meta content="shadow-robot/sr-taco" name="octolytics-dimension-repository_nwo" /><meta content="true" name="octolytics-dimension-repository_public" /><meta content="false" name="octolytics-dimension-repository_is_fork" /><meta content="7731364" name="octolytics-dimension-repository_network_root_id" /><meta content="shadow-robot/sr-taco" name="octolytics-dimension-repository_network_root_nwo" />
  <link href="https://github.com/shadow-robot/sr-taco/commits/F_moveit.atom" rel="alternate" title="Recent Commits to sr-taco:F_moveit" type="application/atom+xml" />

  </head>


  <body class="logged_in page-blob linux vis-public env-production  kill-the-chrome">

    <div class="wrapper">
      
      
      

      <div class="header header-logged-in true">
  <div class="container clearfix">

    <a class="header-logo-invertocat" href="https://github.com/">
  <span class="mega-octicon octicon-mark-github"></span>
</a>

    <div class="divider-vertical"></div>

    
  <a href="/notifications" class="notification-indicator tooltipped downwards" title="You have unread notifications">
    <span class="mail-status unread"></span>
  </a>
  <div class="divider-vertical"></div>


      <div class="command-bar js-command-bar  in-repository">
          <form accept-charset="UTF-8" action="/search" class="command-bar-form" id="top_search_form" method="get">

<input type="text" data-hotkey="/ s" name="q" id="js-command-bar-field" placeholder="Search or type a command" tabindex="1" autocapitalize="off"
    
    data-username="christopherKorpela"
      data-repo="shadow-robot/sr-taco"
      data-branch="F_moveit"
      data-sha="b91ac4172a196d5d40ddf062bfc81562c0a8afd7"
  >

    <input type="hidden" name="nwo" value="shadow-robot/sr-taco" />

    <div class="select-menu js-menu-container js-select-menu search-context-select-menu">
      <span class="minibutton select-menu-button js-menu-target">
        <span class="js-select-button">This repository</span>
      </span>

      <div class="select-menu-modal-holder js-menu-content js-navigation-container">
        <div class="select-menu-modal">

          <div class="select-menu-item js-navigation-item selected">
            <span class="select-menu-item-icon octicon octicon-check"></span>
            <input type="radio" class="js-search-this-repository" name="search_target" value="repository" checked="checked" />
            <div class="select-menu-item-text js-select-button-text">This repository</div>
          </div> <!-- /.select-menu-item -->

          <div class="select-menu-item js-navigation-item">
            <span class="select-menu-item-icon octicon octicon-check"></span>
            <input type="radio" name="search_target" value="global" />
            <div class="select-menu-item-text js-select-button-text">All repositories</div>
          </div> <!-- /.select-menu-item -->

        </div>
      </div>
    </div>

  <span class="octicon help tooltipped downwards" title="Show command bar help">
    <span class="octicon octicon-question"></span>
  </span>


  <input type="hidden" name="ref" value="cmdform">

</form>
        <ul class="top-nav">
            <li class="explore"><a href="/explore">Explore</a></li>
            <li><a href="https://gist.github.com">Gist</a></li>
            <li><a href="/blog">Blog</a></li>
          <li><a href="https://help.github.com">Help</a></li>
        </ul>
      </div>

    

  

    <ul id="user-links">
      <li>
        <a href="/christopherKorpela" class="name">
          <img height="20" src="https://secure.gravatar.com/avatar/3732706168b9ca1233bfcd8952280907?s=140&amp;d=https://a248.e.akamai.net/assets.github.com%2Fimages%2Fgravatars%2Fgravatar-user-420.png" width="20" /> christopherKorpela
        </a>
      </li>

        <li>
          <a href="/new" id="new_repo" class="tooltipped downwards" title="Create a new repo">
            <span class="octicon octicon-repo-create"></span>
          </a>
        </li>

        <li>
          <a href="/settings/profile" id="account_settings"
            class="tooltipped downwards"
            title="Account settings ">
            <span class="octicon octicon-tools"></span>
          </a>
        </li>
        <li>
          <a class="tooltipped downwards" href="/logout" data-method="post" id="logout" title="Sign out">
            <span class="octicon octicon-log-out"></span>
          </a>
        </li>

    </ul>


<div class="js-new-dropdown-contents hidden">
  

<ul class="dropdown-menu">
  <li>
    <a href="/new"><span class="octicon octicon-repo-create"></span> New repository</a>
  </li>
  <li>
    <a href="/organizations/new"><span class="octicon octicon-list-unordered"></span> New organization</a>
  </li>



    <li class="section-title">
      <span title="shadow-robot/sr-taco">This repository</span>
    </li>
    <li>
      <a href="/shadow-robot/sr-taco/issues/new"><span class="octicon octicon-issue-opened"></span> New issue</a>
    </li>
</ul>

</div>


    
  </div>
</div>

      

      




          <div class="site" itemscope itemtype="http://schema.org/WebPage">
    
    <div class="pagehead repohead instapaper_ignore readability-menu">
      <div class="container">
        

<ul class="pagehead-actions">

    <li class="subscription">
      <form accept-charset="UTF-8" action="/notifications/subscribe" data-autosubmit="true" data-remote="true" method="post"><div style="margin:0;padding:0;display:inline"><input name="authenticity_token" type="hidden" value="hCZAZTvQyHpLdZSvXSYgNXDR1l7Fv35gtv33kB091TQ=" /></div>  <input id="repository_id" name="repository_id" type="hidden" value="7731364" />

    <div class="select-menu js-menu-container js-select-menu">
      <span class="minibutton select-menu-button  js-menu-target">
        <span class="js-select-button">
          <span class="octicon octicon-eye-watch"></span>
          Watch
        </span>
      </span>

      <div class="select-menu-modal-holder">
        <div class="select-menu-modal subscription-menu-modal js-menu-content">
          <div class="select-menu-header">
            <span class="select-menu-title">Notification status</span>
            <span class="octicon octicon-remove-close js-menu-close"></span>
          </div> <!-- /.select-menu-header -->

          <div class="select-menu-list js-navigation-container">

            <div class="select-menu-item js-navigation-item selected">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <div class="select-menu-item-text">
                <input checked="checked" id="do_included" name="do" type="radio" value="included" />
                <h4>Not watching</h4>
                <span class="description">You only receive notifications for discussions in which you participate or are @mentioned.</span>
                <span class="js-select-button-text hidden-select-button-text">
                  <span class="octicon octicon-eye-watch"></span>
                  Watch
                </span>
              </div>
            </div> <!-- /.select-menu-item -->

            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon octicon-check"></span>
              <div class="select-menu-item-text">
                <input id="do_subscribed" name="do" type="radio" value="subscribed" />
                <h4>Watching</h4>
                <span class="description">You receive notifications for all discussions in this repository.</span>
                <span class="js-select-button-text hidden-select-button-text">
                  <span class="octicon octicon-eye-unwatch"></span>
                  Unwatch
                </span>
              </div>
            </div> <!-- /.select-menu-item -->

            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <div class="select-menu-item-text">
                <input id="do_ignore" name="do" type="radio" value="ignore" />
                <h4>Ignoring</h4>
                <span class="description">You do not receive any notifications for discussions in this repository.</span>
                <span class="js-select-button-text hidden-select-button-text">
                  <span class="octicon octicon-mute"></span>
                  Stop ignoring
                </span>
              </div>
            </div> <!-- /.select-menu-item -->

          </div> <!-- /.select-menu-list -->

        </div> <!-- /.select-menu-modal -->
      </div> <!-- /.select-menu-modal-holder -->
    </div> <!-- /.select-menu -->

</form>
    </li>

    <li class="js-toggler-container js-social-container starring-container ">
      <a href="/shadow-robot/sr-taco/unstar" class="minibutton with-count js-toggler-target star-button starred upwards" title="Unstar this repo" data-remote="true" data-method="post" rel="nofollow">
        <span class="octicon octicon-star-delete"></span>
        <span class="text">Unstar</span>
      </a>
      <a href="/shadow-robot/sr-taco/star" class="minibutton with-count js-toggler-target star-button unstarred upwards" title="Star this repo" data-remote="true" data-method="post" rel="nofollow">
        <span class="octicon octicon-star"></span>
        <span class="text">Star</span>
      </a>
      <a class="social-count js-social-count" href="/shadow-robot/sr-taco/stargazers">0</a>
    </li>

        <li>
          <a href="/shadow-robot/sr-taco/fork" class="minibutton with-count js-toggler-target fork-button lighter upwards" title="Fork this repo" rel="facebox nofollow">
            <span class="octicon octicon-git-branch-create"></span>
            <span class="text">Fork</span>
          </a>
          <a href="/shadow-robot/sr-taco/network" class="social-count">0</a>
        </li>


</ul>

        <h1 itemscope itemtype="http://data-vocabulary.org/Breadcrumb" class="entry-title public">
          <span class="repo-label"><span>public</span></span>
          <span class="mega-octicon octicon-repo"></span>
          <span class="author">
            <a href="/shadow-robot" class="url fn" itemprop="url" rel="author"><span itemprop="title">shadow-robot</span></a></span
          ><span class="repohead-name-divider">/</span><strong
          ><a href="/shadow-robot/sr-taco" class="js-current-repository js-repo-home-link">sr-taco</a></strong>

          <span class="page-context-loader">
            <img alt="Octocat-spinner-32" height="16" src="https://a248.e.akamai.net/assets.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
          </span>

        </h1>
      </div><!-- /.container -->
    </div><!-- /.repohead -->

    <div class="container">

      <div class="repository-with-sidebar repo-container
            ">

          <div class="repository-sidebar">

              

<div class="repo-nav repo-nav-full js-repository-container-pjax js-octicon-loaders">
  <div class="repo-nav-contents">
    <ul class="repo-menu">
      <li class="tooltipped leftwards" title="Code">
        <a href="/shadow-robot/sr-taco/tree/F_moveit" class="js-selected-navigation-item selected" data-gotokey="c" data-pjax="true" data-selected-links="repo_source repo_downloads repo_commits repo_tags repo_branches /shadow-robot/sr-taco/tree/F_moveit">
          <span class="octicon octicon-code"></span> <span class="full-word">Code</span>
          <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://a248.e.akamai.net/assets.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>

        <li class="tooltipped leftwards" title="Issues">
          <a href="/shadow-robot/sr-taco/issues" class="js-selected-navigation-item js-disable-pjax" data-gotokey="i" data-selected-links="repo_issues /shadow-robot/sr-taco/issues">
            <span class="octicon octicon-issue-opened"></span> <span class="full-word">Issues</span>
            <span class='counter'>5</span>
            <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://a248.e.akamai.net/assets.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
</a>        </li>

      <li class="tooltipped leftwards" title="Pull Requests"><a href="/shadow-robot/sr-taco/pulls" class="js-selected-navigation-item js-disable-pjax" data-gotokey="p" data-selected-links="repo_pulls /shadow-robot/sr-taco/pulls">
            <span class="octicon octicon-git-pull-request"></span> <span class="full-word">Pull Requests</span>
            <span class='counter'>0</span>
            <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://a248.e.akamai.net/assets.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>


        <li class="tooltipped leftwards" title="Wiki">
          <a href="/shadow-robot/sr-taco/wiki" class="js-selected-navigation-item " data-pjax="true" data-selected-links="repo_wiki /shadow-robot/sr-taco/wiki">
            <span class="octicon octicon-book"></span> <span class="full-word">Wiki</span>
            <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://a248.e.akamai.net/assets.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
</a>        </li>


    </ul>
    <div class="repo-menu-separator"></div>
    <ul class="repo-menu">

      <li class="tooltipped leftwards" title="Pulse">
        <a href="/shadow-robot/sr-taco/pulse" class="js-selected-navigation-item " data-pjax="true" data-selected-links="pulse /shadow-robot/sr-taco/pulse">
          <span class="octicon octicon-pulse"></span> <span class="full-word">Pulse</span>
          <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://a248.e.akamai.net/assets.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>

      <li class="tooltipped leftwards" title="Graphs">
        <a href="/shadow-robot/sr-taco/graphs" class="js-selected-navigation-item " data-pjax="true" data-selected-links="repo_graphs repo_contributors /shadow-robot/sr-taco/graphs">
          <span class="octicon octicon-graph"></span> <span class="full-word">Graphs</span>
          <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://a248.e.akamai.net/assets.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>

      <li class="tooltipped leftwards" title="Network">
        <a href="/shadow-robot/sr-taco/network" class="js-selected-navigation-item js-disable-pjax" data-selected-links="repo_network /shadow-robot/sr-taco/network">
          <span class="octicon octicon-git-branch"></span> <span class="full-word">Network</span>
          <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://a248.e.akamai.net/assets.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>

    </ul>

  </div>
</div>


              <div class="only-with-full-nav">

                

  

<div class="clone-url open"
  data-protocol-type="http"
  data-url="/users/set_protocol?protocol_selector=http&amp;protocol_type=clone">
  <h3><strong>HTTPS</strong> clone URL</h3>

  <input type="text" class="clone js-url-field"
         value="https://github.com/shadow-robot/sr-taco.git" readonly="readonly">

  <span class="js-zeroclipboard url-box-clippy minibutton zeroclipboard-button" data-clipboard-text="https://github.com/shadow-robot/sr-taco.git" data-copied-hint="copied!" title="copy to clipboard"><span class="octicon octicon-clippy"></span></span>
</div>

  

<div class="clone-url "
  data-protocol-type="ssh"
  data-url="/users/set_protocol?protocol_selector=ssh&amp;protocol_type=clone">
  <h3><strong>SSH</strong> clone URL</h3>

  <input type="text" class="clone js-url-field"
         value="git@github.com:shadow-robot/sr-taco.git" readonly="readonly">

  <span class="js-zeroclipboard url-box-clippy minibutton zeroclipboard-button" data-clipboard-text="git@github.com:shadow-robot/sr-taco.git" data-copied-hint="copied!" title="copy to clipboard"><span class="octicon octicon-clippy"></span></span>
</div>

  

<div class="clone-url "
  data-protocol-type="subversion"
  data-url="/users/set_protocol?protocol_selector=subversion&amp;protocol_type=clone">
  <h3><strong>Subversion</strong> checkout URL</h3>

  <input type="text" class="clone js-url-field"
         value="https://github.com/shadow-robot/sr-taco" readonly="readonly">

  <span class="js-zeroclipboard url-box-clippy minibutton zeroclipboard-button" data-clipboard-text="https://github.com/shadow-robot/sr-taco" data-copied-hint="copied!" title="copy to clipboard"><span class="octicon octicon-clippy"></span></span>
</div>



<p class="clone-options">You can clone with
    <a href="#" class="js-clone-selector" data-protocol="http">HTTPS</a>,
    <a href="#" class="js-clone-selector" data-protocol="ssh">SSH</a>,
    <a href="#" class="js-clone-selector" data-protocol="subversion">Subversion</a>,
  and <a href="https://help.github.com/articles/which-remote-url-should-i-use">other methods.</a>
</p>




                  <a href="/shadow-robot/sr-taco/archive/F_moveit.zip"
                     class="minibutton sidebar-button"
                     title="Download this repository as a zip file"
                     rel="nofollow">
                    <span class="octicon octicon-cloud-download"></span>
                    Download ZIP
                  </a>

              </div>
          </div>

          <div id="js-repo-pjax-container" class="repository-content context-loader-container" data-pjax-container>
            


<!-- blob contrib key: blob_contributors:v21:f2c1a81db3588dcf658e9ff28c46816a -->
<!-- blob contrib frag key: views10/v8/blob_contributors:v21:f2c1a81db3588dcf658e9ff28c46816a -->


      <p title="This is a placeholder element" class="js-history-link-replace hidden"></p>

        <a href="/shadow-robot/sr-taco/find/F_moveit" data-pjax data-hotkey="t" style="display:none">Show File Finder</a>

        <div class="file-navigation">
          


<div class="select-menu js-menu-container js-select-menu" >
  <span class="minibutton select-menu-button js-menu-target" data-hotkey="w"
    data-master-branch="master"
    data-ref="F_moveit">
    <span class="octicon octicon-git-branch"></span>
    <i>branch:</i>
    <span class="js-select-button">F_moveit</span>
  </span>

  <div class="select-menu-modal-holder js-menu-content js-navigation-container" data-pjax>

    <div class="select-menu-modal">
      <div class="select-menu-header">
        <span class="select-menu-title">Switch branches/tags</span>
        <span class="octicon octicon-remove-close js-menu-close"></span>
      </div> <!-- /.select-menu-header -->

      <div class="select-menu-filters">
        <div class="select-menu-text-filter">
          <input type="text" id="context-commitish-filter-field" class="js-filterable-field js-navigation-enable" placeholder="Filter branches/tags">
        </div>
        <div class="select-menu-tabs">
          <ul>
            <li class="select-menu-tab">
              <a href="#" data-tab-filter="branches" class="js-select-menu-tab">Branches</a>
            </li>
            <li class="select-menu-tab">
              <a href="#" data-tab-filter="tags" class="js-select-menu-tab">Tags</a>
            </li>
          </ul>
        </div><!-- /.select-menu-tabs -->
      </div><!-- /.select-menu-filters -->

      <div class="select-menu-list select-menu-tab-bucket js-select-menu-tab-bucket" data-tab-filter="branches">

        <div data-filterable-for="context-commitish-filter-field" data-filterable-type="substring">


            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/shadow-robot/sr-taco/blob/F_bride_pkg/sr_visual_servoing/include/sr_visual_servoing/utils.hpp" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="F_bride_pkg" rel="nofollow" title="F_bride_pkg">F_bride_pkg</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/shadow-robot/sr-taco/blob/F_connect_tracker_servo/sr_visual_servoing/include/sr_visual_servoing/utils.hpp" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="F_connect_tracker_servo" rel="nofollow" title="F_connect_tracker_servo">F_connect_tracker_servo</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/shadow-robot/sr-taco/blob/F_launch_real/sr_visual_servoing/include/sr_visual_servoing/utils.hpp" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="F_launch_real" rel="nofollow" title="F_launch_real">F_launch_real</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item selected">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/shadow-robot/sr-taco/blob/F_moveit/sr_visual_servoing/include/sr_visual_servoing/utils.hpp" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="F_moveit" rel="nofollow" title="F_moveit">F_moveit</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/shadow-robot/sr-taco/blob/F_rqt_servo_gui/sr_visual_servoing/include/sr_visual_servoing/utils.hpp" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="F_rqt_servo_gui" rel="nofollow" title="F_rqt_servo_gui">F_rqt_servo_gui</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/shadow-robot/sr-taco/blob/master/sr_visual_servoing/include/sr_visual_servoing/utils.hpp" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="master" rel="nofollow" title="master">master</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/shadow-robot/sr-taco/blob/test1/sr_visual_servoing/include/sr_visual_servoing/utils.hpp" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="test1" rel="nofollow" title="test1">test1</a>
            </div> <!-- /.select-menu-item -->
        </div>

          <div class="select-menu-no-results">Nothing to show</div>
      </div> <!-- /.select-menu-list -->

      <div class="select-menu-list select-menu-tab-bucket js-select-menu-tab-bucket" data-tab-filter="tags">
        <div data-filterable-for="context-commitish-filter-field" data-filterable-type="substring">


        </div>

        <div class="select-menu-no-results">Nothing to show</div>
      </div> <!-- /.select-menu-list -->

    </div> <!-- /.select-menu-modal -->
  </div> <!-- /.select-menu-modal-holder -->
</div> <!-- /.select-menu -->

          <div class="breadcrumb">
            <span class='repo-root js-repo-root'><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/shadow-robot/sr-taco/tree/F_moveit" data-branch="F_moveit" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">sr-taco</span></a></span></span><span class="separator"> / </span><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/shadow-robot/sr-taco/tree/F_moveit/sr_visual_servoing" data-branch="F_moveit" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">sr_visual_servoing</span></a></span><span class="separator"> / </span><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/shadow-robot/sr-taco/tree/F_moveit/sr_visual_servoing/include" data-branch="F_moveit" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">include</span></a></span><span class="separator"> / </span><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/shadow-robot/sr-taco/tree/F_moveit/sr_visual_servoing/include/sr_visual_servoing" data-branch="F_moveit" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">sr_visual_servoing</span></a></span><span class="separator"> / </span><strong class="final-path">utils.hpp</strong> <span class="js-zeroclipboard minibutton zeroclipboard-button" data-clipboard-text="sr_visual_servoing/include/sr_visual_servoing/utils.hpp" data-copied-hint="copied!" title="copy to clipboard"><span class="octicon octicon-clippy"></span></span>
          </div>
        </div>


        <div class="commit commit-loader file-history-tease js-deferred-content" data-url="/shadow-robot/sr-taco/contributors/F_moveit/sr_visual_servoing/include/sr_visual_servoing/utils.hpp">
          Fetching contributors…

          <div class="participation">
            <p class="loader-loading"><img alt="Octocat-spinner-32-eaf2f5" height="16" src="https://a248.e.akamai.net/assets.github.com/images/spinners/octocat-spinner-32-EAF2F5.gif" width="16" /></p>
            <p class="loader-error">Cannot retrieve contributors at this time</p>
          </div>
        </div>


        <div id="files" class="bubble">
          <div class="file">
            <div class="meta">
              <div class="info">
                <span class="icon"><b class="octicon octicon-file-text"></b></span>
                <span class="mode" title="File Mode">file</span>
                  <span>54 lines (47 sloc)</span>
                <span>1.524 kb</span>
              </div>
              <div class="actions">
                <div class="button-group">
                        <a class="minibutton tooltipped leftwards"
                           title="Clicking this button will automatically fork this project so you can edit the file"
                           href="/shadow-robot/sr-taco/edit/F_moveit/sr_visual_servoing/include/sr_visual_servoing/utils.hpp"
                           data-method="post" rel="nofollow">Edit</a>
                  <a href="/shadow-robot/sr-taco/raw/F_moveit/sr_visual_servoing/include/sr_visual_servoing/utils.hpp" class="button minibutton " id="raw-url">Raw</a>
                    <a href="/shadow-robot/sr-taco/blame/F_moveit/sr_visual_servoing/include/sr_visual_servoing/utils.hpp" class="button minibutton ">Blame</a>
                  <a href="/shadow-robot/sr-taco/commits/F_moveit/sr_visual_servoing/include/sr_visual_servoing/utils.hpp" class="button minibutton " rel="nofollow">History</a>
                </div><!-- /.button-group -->
              </div><!-- /.actions -->

            </div>
                <div class="blob-wrapper data type-c js-blob-data">
      <table class="file-code file-diff">
        <tr class="file-code-line">
          <td class="blob-line-nums">
            <span id="L1" rel="#L1">1</span>
<span id="L2" rel="#L2">2</span>
<span id="L3" rel="#L3">3</span>
<span id="L4" rel="#L4">4</span>
<span id="L5" rel="#L5">5</span>
<span id="L6" rel="#L6">6</span>
<span id="L7" rel="#L7">7</span>
<span id="L8" rel="#L8">8</span>
<span id="L9" rel="#L9">9</span>
<span id="L10" rel="#L10">10</span>
<span id="L11" rel="#L11">11</span>
<span id="L12" rel="#L12">12</span>
<span id="L13" rel="#L13">13</span>
<span id="L14" rel="#L14">14</span>
<span id="L15" rel="#L15">15</span>
<span id="L16" rel="#L16">16</span>
<span id="L17" rel="#L17">17</span>
<span id="L18" rel="#L18">18</span>
<span id="L19" rel="#L19">19</span>
<span id="L20" rel="#L20">20</span>
<span id="L21" rel="#L21">21</span>
<span id="L22" rel="#L22">22</span>
<span id="L23" rel="#L23">23</span>
<span id="L24" rel="#L24">24</span>
<span id="L25" rel="#L25">25</span>
<span id="L26" rel="#L26">26</span>
<span id="L27" rel="#L27">27</span>
<span id="L28" rel="#L28">28</span>
<span id="L29" rel="#L29">29</span>
<span id="L30" rel="#L30">30</span>
<span id="L31" rel="#L31">31</span>
<span id="L32" rel="#L32">32</span>
<span id="L33" rel="#L33">33</span>
<span id="L34" rel="#L34">34</span>
<span id="L35" rel="#L35">35</span>
<span id="L36" rel="#L36">36</span>
<span id="L37" rel="#L37">37</span>
<span id="L38" rel="#L38">38</span>
<span id="L39" rel="#L39">39</span>
<span id="L40" rel="#L40">40</span>
<span id="L41" rel="#L41">41</span>
<span id="L42" rel="#L42">42</span>
<span id="L43" rel="#L43">43</span>
<span id="L44" rel="#L44">44</span>
<span id="L45" rel="#L45">45</span>
<span id="L46" rel="#L46">46</span>
<span id="L47" rel="#L47">47</span>
<span id="L48" rel="#L48">48</span>
<span id="L49" rel="#L49">49</span>
<span id="L50" rel="#L50">50</span>
<span id="L51" rel="#L51">51</span>
<span id="L52" rel="#L52">52</span>
<span id="L53" rel="#L53">53</span>

          </td>
          <td class="blob-line-code">
                  <div class="highlight"><pre><div class='line' id='LC1'><span class="cm">/**</span></div><div class='line' id='LC2'><span class="cm"> * @file   analyse_moving_object.hpp</span></div><div class='line' id='LC3'><span class="cm"> * @author Ugo Cupcic &lt;ugo@shadowrobot.com&gt;</span></div><div class='line' id='LC4'><span class="cm"> * @date   Tue Sep  4 09:44:09 2012</span></div><div class='line' id='LC5'><span class="cm"> *</span></div><div class='line' id='LC6'><span class="cm">*</span></div><div class='line' id='LC7'><span class="cm">* Copyright 2011 Shadow Robot Company Ltd.</span></div><div class='line' id='LC8'><span class="cm">*</span></div><div class='line' id='LC9'><span class="cm">* This program is free software: you can redistribute it and/or modify it</span></div><div class='line' id='LC10'><span class="cm">* under the terms of the GNU General Public License as published by the Free</span></div><div class='line' id='LC11'><span class="cm">* Software Foundation, either version 2 of the License, or (at your option)</span></div><div class='line' id='LC12'><span class="cm">* any later version.</span></div><div class='line' id='LC13'><span class="cm">*</span></div><div class='line' id='LC14'><span class="cm">* This program is distributed in the hope that it will be useful, but WITHOUT</span></div><div class='line' id='LC15'><span class="cm">* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or</span></div><div class='line' id='LC16'><span class="cm">* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for</span></div><div class='line' id='LC17'><span class="cm">* more details.</span></div><div class='line' id='LC18'><span class="cm">*</span></div><div class='line' id='LC19'><span class="cm">* You should have received a copy of the GNU General Public License along</span></div><div class='line' id='LC20'><span class="cm">* with this program.  If not, see &lt;http://www.gnu.org/licenses/&gt;.</span></div><div class='line' id='LC21'><span class="cm">*</span></div><div class='line' id='LC22'><span class="cm"> * @brief  This node is used to analyse a moving object. It publishes important information</span></div><div class='line' id='LC23'><span class="cm"> * (speed / acceleration) and also publishes useful markers for displaying in rviz.</span></div><div class='line' id='LC24'><span class="cm"> *</span></div><div class='line' id='LC25'><span class="cm"> */</span></div><div class='line' id='LC26'><br/></div><div class='line' id='LC27'><span class="cp">#ifndef _UTILS_GRASP_MOVING_OBJECT_HPP_</span></div><div class='line' id='LC28'><span class="cp">#define _UTILS_GRASP_MOVING_OBJECT_HPP_</span></div><div class='line' id='LC29'><br/></div><div class='line' id='LC30'><span class="cp">#include &lt;ros/ros.h&gt;</span></div><div class='line' id='LC31'><span class="cp">#include &lt;geometry_msgs/Point.h&gt;</span></div><div class='line' id='LC32'><span class="cp">#include &quot;tf/transform_datatypes.h&quot;</span></div><div class='line' id='LC33'><br/></div><div class='line' id='LC34'><span class="k">namespace</span> <span class="n">sr_utils</span></div><div class='line' id='LC35'><span class="p">{</span></div><div class='line' id='LC36'>&nbsp;&nbsp;<span class="k">static</span> <span class="kr">inline</span> <span class="kt">double</span> <span class="n">compute_distance</span><span class="p">(</span><span class="n">geometry_msgs</span><span class="o">::</span><span class="n">Point</span> <span class="n">a</span><span class="p">,</span> <span class="n">geometry_msgs</span><span class="o">::</span><span class="n">Point</span> <span class="n">b</span><span class="p">)</span></div><div class='line' id='LC37'>&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC38'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">tf</span><span class="o">::</span><span class="n">Vector3</span> <span class="n">pt1</span><span class="p">,</span> <span class="n">pt2</span><span class="p">;</span></div><div class='line' id='LC39'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">pointMsgToTF</span><span class="p">(</span><span class="n">a</span><span class="p">,</span> <span class="n">pt1</span><span class="p">);</span></div><div class='line' id='LC40'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">pointMsgToTF</span><span class="p">(</span><span class="n">b</span><span class="p">,</span> <span class="n">pt2</span><span class="p">);</span></div><div class='line' id='LC41'><br/></div><div class='line' id='LC42'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">double</span> <span class="n">distance</span> <span class="o">=</span> <span class="n">pt1</span><span class="p">.</span><span class="n">distance</span><span class="p">(</span><span class="n">pt2</span><span class="p">);</span></div><div class='line' id='LC43'><br/></div><div class='line' id='LC44'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">return</span> <span class="n">distance</span><span class="p">;</span></div><div class='line' id='LC45'>&nbsp;&nbsp;<span class="p">};</span></div><div class='line' id='LC46'><span class="p">}</span></div><div class='line' id='LC47'><span class="cm">/* For the emacs weenies in the crowd.</span></div><div class='line' id='LC48'><span class="cm">   Local Variables:</span></div><div class='line' id='LC49'><span class="cm">   c-basic-offset: 2</span></div><div class='line' id='LC50'><span class="cm">   End:</span></div><div class='line' id='LC51'><span class="cm">*/</span></div><div class='line' id='LC52'><br/></div><div class='line' id='LC53'><span class="cp">#endif</span></div></pre></div>
          </td>
        </tr>
      </table>
  </div>

          </div>
        </div>

        <a href="#jump-to-line" rel="facebox[.linejump]" data-hotkey="l" class="js-jump-to-line" style="display:none">Jump to Line</a>
        <div id="jump-to-line" style="display:none">
          <form accept-charset="UTF-8" class="js-jump-to-line-form">
            <input class="linejump-input js-jump-to-line-field" type="text" placeholder="Jump to line&hellip;">
            <button type="submit" class="button">Go</button>
          </form>
        </div>

</div>

<div id="js-frame-loading-template" class="frame frame-loading large-loading-area" style="display:none;">
  <img class="js-frame-loading-spinner" src="https://a248.e.akamai.net/assets.github.com/images/spinners/octocat-spinner-128.gif" height="64" width="64">
</div>


          </div>
        </div>

      </div><!-- /.repo-container -->
      <div class="modal-backdrop"></div>
    </div>
  </div><!-- /.site -->


    </div><!-- /.wrapper -->

      <div class="container">
  <div class="site-footer">
    <ul class="site-footer-links right">
      <li><a href="https://status.github.com/">Status</a></li>
      <li><a href="http://developer.github.com">Developer</a></li>
      <li><a href="http://training.github.com">Training</a></li>
      <li><a href="http://shop.github.com">Shop</a></li>
      <li><a href="/blog">Blog</a></li>
      <li><a href="/about">About</a></li>
    </ul>

    <a href="/">
      <span class="mega-octicon octicon-mark-github"></span>
    </a>

    <ul class="site-footer-links">
      <li>&copy; 2013 <span title="0.13780s from fe13.rs.github.com">GitHub</span>, Inc.</li>
        <li><a href="/site/terms">Terms</a></li>
        <li><a href="/site/privacy">Privacy</a></li>
        <li><a href="/security">Security</a></li>
        <li><a href="/contact">Contact</a></li>
    </ul>
  </div><!-- /.site-footer -->
</div><!-- /.container -->


    <div class="fullscreen-overlay js-fullscreen-overlay" id="fullscreen_overlay">
  <div class="fullscreen-container js-fullscreen-container">
    <div class="textarea-wrap">
      <textarea name="fullscreen-contents" id="fullscreen-contents" class="js-fullscreen-contents" placeholder="" data-suggester="fullscreen_suggester"></textarea>
          <div class="suggester-container">
              <div class="suggester fullscreen-suggester js-navigation-container" id="fullscreen_suggester"
                 data-url="/shadow-robot/sr-taco/suggestions/commit">
              </div>
          </div>
    </div>
  </div>
  <div class="fullscreen-sidebar">
    <a href="#" class="exit-fullscreen js-exit-fullscreen tooltipped leftwards" title="Exit Zen Mode">
      <span class="mega-octicon octicon-screen-normal"></span>
    </a>
    <a href="#" class="theme-switcher js-theme-switcher tooltipped leftwards"
      title="Switch themes">
      <span class="octicon octicon-color-mode"></span>
    </a>
  </div>
</div>



    <div id="ajax-error-message" class="flash flash-error">
      <span class="octicon octicon-alert"></span>
      <a href="#" class="octicon octicon-remove-close close ajax-error-dismiss"></a>
      Something went wrong with that request. Please try again.
    </div>

    
    <span id='server_response_time' data-time='0.13823' data-host='fe13'></span>
    
  </body>
</html>

