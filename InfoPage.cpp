﻿#include "pch.h"
#include "InfoPage.h"
#if __has_include("InfoPage.g.cpp")
#include "InfoPage.g.cpp"
#endif

using namespace winrt;
using namespace Windows::ApplicationModel::DataTransfer;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;

namespace winrt::bikabika::implementation
{
    InfoPage::InfoPage()
    {
        InitializeComponent();
        NavigationCacheMode(Windows::UI::Xaml::Navigation::NavigationCacheMode::Enabled);
    }

    Windows::Foundation::IAsyncAction InfoPage::ContentDialogShow(hstring const& mode, hstring const& message)
    {
        Windows::ApplicationModel::Resources::ResourceLoader resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() };
        if (mode == L"Timeout") {
            auto show{ PicErrorDialog().ShowAsync() };
        }
        else {
            HttpContentDialog().Title(box_value(resourceLoader.GetString(L"FailHttpTitle")));
            HttpContentDialog().CloseButtonText(resourceLoader.GetString(L"FailCloseButtonText"));
            if (mode == L"Error")
            {
                HttpContentDialog().Content(box_value(message));
                auto show{ co_await HttpContentDialog().ShowAsync() };

            }
            else if (mode == L"Unknown")
            {
                Windows::Data::Json::JsonObject resp = Windows::Data::Json::JsonObject::Parse(message);
                HttpContentDialog().Content(box_value(to_hstring(resp.GetNamedNumber(L"code")) + L":" + resp.GetNamedString(L"message")));
                auto show{ co_await HttpContentDialog().ShowAsync() };

            }
            else if (mode == L"1005")
            {
                HttpContentDialog().Content(box_value(resourceLoader.GetString(L"FailAuth")));
                extern bool m_login;
                m_login = false;
                auto show{ co_await HttpContentDialog().ShowAsync() };
                if (show == winrt::Windows::UI::Xaml::Controls::ContentDialogResult::None)
                {
                    auto loginTeachingTip = Frame().Parent().as<Microsoft::UI::Xaml::Controls::NavigationView>().Parent().as<Windows::UI::Xaml::Controls::Grid>().Children().GetAt(3).as<Microsoft::UI::Xaml::Controls::TeachingTip>();
                    Windows::ApplicationModel::Resources::ResourceLoader resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() };
                    loginTeachingTip.Title(resourceLoader.GetString(L"LoginButton/Content"));
                    loginTeachingTip.IsOpen(true);
                }
            }
        }
        
    }
    Windows::Foundation::IAsyncAction InfoPage::Eps(int32_t const& page)
    {
        auto res{ co_await m_bikaHttp.Episodes(m_id,page) };
        if (res[1] == 'T')
        {
            co_await ContentDialogShow(L"Timeout", L"");
        }
        else if (res[1] == 'E') {
            co_await ContentDialogShow(L"Error", res);
        }
        else
        {
            Windows::Data::Json::JsonObject resp = Windows::Data::Json::JsonObject::Parse(res);
            double code = resp.GetNamedNumber(L"code");
            if (code == (double)200)
            {
                Windows::Data::Json::JsonObject ca = resp.GetNamedObject(L"data");
                Windows::Data::Json::JsonObject eps = ca.GetNamedObject(L"eps");
                m_epsTotal = eps.GetNamedNumber(L"total");
                m_epsLimit = eps.GetNamedNumber(L"limit");
                m_epsPage = eps.GetNamedNumber(L"page");
                m_epsPages = eps.GetNamedNumber(L"pages");
                for (auto x : eps.GetNamedArray(L"docs"))
                {
                    Windows::Data::Json::JsonObject json = x.GetObject();
                    auto epi = winrt::make<EpisodeBlock>(json.GetNamedString(L"_id"), json.GetNamedString(L"title"), json.GetNamedNumber(L"order"), json.GetNamedString(L"updated_at"));
                    epi.BookId(m_id);
                    epi.Total(m_epsTotal);
                    m_eps.Append(epi);
                    
                }

                if (m_epsPages > m_epsPage) {
                    co_await Eps(m_epsPage + 1);
                }
                else if (m_epsPages == m_epsPage)
                {
                    auto resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() };
                    auto counts = eps.GetNamedArray(L"docs").Size();
                    hstring read = resourceLoader.GetString(L"ReadText");
                    auto epiLast = winrt::make<EpisodeBlock>(eps.GetNamedArray(L"docs").GetObjectAt(counts - 1).GetNamedString(L"_id"), read, eps.GetNamedArray(L"docs").GetObjectAt(counts - 1).GetNamedNumber(L"order"), eps.GetNamedArray(L"docs").GetObjectAt(counts - 1).GetNamedString(L"updated_at"));
                    epiLast.BookId(m_id);
                    epiLast.Total(m_epsTotal);
                    //epiLast.Title(resourceLoader.GetString(L"ReadText"));
                    m_eps.InsertAt(0, epiLast);
                    ListV().ItemsSource(box_value(m_eps));
                }
                
            }
            //缺少鉴权
            else if (code == (double)401 && resp.GetNamedString(L"error") == L"1005")
            {
                co_await ContentDialogShow(L"1005", L"");
            }
            //未知
            else
            {
                co_await ContentDialogShow(L"Unknown", res);
            }
        }
    }
    void InfoPage::OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e)
    {
        //bug 多次使用标签搜索后返回导致动画img冲突
        //todo 改用标签页
        //winrt::Windows::UI::Xaml::Media::Animation::ConnectedAnimationService::GetForCurrentView().PrepareToAnimate(L"BackConnectedAnimation", Img());
        __super::OnNavigatedFrom(e);
    }
    
    Windows::Foundation::IAsyncAction InfoPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e)
    {
        auto params = winrt::unbox_value<winrt::Windows::Foundation::Collections::IVector<winrt::Windows::Foundation::IInspectable>>(e.Parameter());
        auto img = winrt::unbox_value<winrt::Windows::UI::Xaml::Media::ImageSource>(params.GetAt(0));
		m_id = winrt::unbox_value<hstring>(params.GetAt(1));
		__super::OnNavigatedTo(e);
        extern bool animeFlag;
        auto anim = winrt::Windows::UI::Xaml::Media::Animation::ConnectedAnimationService::GetForCurrentView().GetAnimation(L"ForwardConnectedAnimation");
        if (anim)
        {
            Img().Source(img);
            anim.TryStart(Img());
        }
        m_tags.Clear();
        animeFlag = false;
        m_eps = winrt::single_threaded_observable_vector<bikabika::EpisodeBlock>();
        auto res{ co_await m_bikaHttp.BookInfo(m_id) };
        if (res[1] == 'T') {
            //auto show{ PicErrorDialog().ShowAsync() };
        }
        else if (res[1] == 'E') {
            /*auto resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() };
            LoginContentDialog().Title(box_value(resourceLoader.GetString(L"LoadFail/Title")));
            LoginContentDialog().Content(box_value(res));
            LoginContentDialog().CloseButtonText(resourceLoader.GetString(L"Fail/CloseButtonText"));
            auto show{ co_await LoginContentDialog().ShowAsync() };*/
        }
        else
        {
            Windows::Data::Json::JsonObject resp = Windows::Data::Json::JsonObject::Parse(res);
            double code = resp.GetNamedNumber(L"code");
            if (code == (double)200)
            {
                Windows::Data::Json::JsonObject ca = resp.GetNamedObject(L"data");
                Windows::Data::Json::JsonObject json = ca.GetNamedObject(L"comic");
                m_id = json.GetNamedString(L"_id");
                m_title = json.GetNamedString(L"title");
                Title().Text(m_title);
                m_creater = winrt::make<CreaterBlock>(json.GetNamedObject(L"_creator"));
                CreaterName().Text(m_creater.Name());
                UsersName().Text(m_creater.Name());
                Creater().ProfilePicture(m_creater.Thumb());
                UsersPic().ProfilePicture(m_creater.Thumb());
                UsersLevel().Text(L"Lv." + to_hstring(m_creater.Level()));
                UsersTitle().Text(m_creater.Title());
                UserInfomation().Text(m_creater.Slogan());
                m_description = json.GetNamedString(L"description");
                Description().Text(m_description);
                Windows::Storage::ApplicationDataContainer serversSettings = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"Servers", Windows::Storage::ApplicationDataCreateDisposition::Always);
                hstring path = unbox_value<winrt::hstring>(serversSettings.Values().Lookup(L"picServersCurrent")) + L"/static/" + json.GetNamedObject(L"thumb").GetNamedString(L"path");
                m_thumb = winrt::Windows::UI::Xaml::Media::Imaging::BitmapImage{ Windows::Foundation::Uri{ path} };
                m_author = json.GetNamedString(L"author");
                Author().Text(m_author);
                if (auto s = json.TryLookup(L"chineseTeam"))
                {
                    m_chineseTeam = s.GetString();
                    ChineseTeam().Text(m_chineseTeam);
                }
                for (auto x : json.GetNamedArray(L"categories"))
                {
                    m_categories.Append(winrt::make<TagBlock>(x.GetString()));
                    m_categoriesString = m_categoriesString + x.GetString() + L"  ";
                }
                CategoriesString().Text(m_categoriesString);
                //! bika服务器问题导致水手服标签存在回车
                for (auto x : json.GetNamedArray(L"tags"))
                {
                    if (L"水手服\r" == x.GetString()) m_tags.Append(winrt::make<TagBlock>(L"水手服"));
                    else m_tags.Append(winrt::make<TagBlock>(x.GetString()));
                    m_tagsString = m_tagsString + x.GetString() + L"  ";
                }
                m_pagesCount = json.GetNamedNumber(L"pagesCount");
                m_epsCount = json.GetNamedNumber(L"epsCount");
                EpsCount().Text(to_hstring(m_epsCount));
                PagesCount().Text(to_hstring(m_pagesCount));
                if (json.GetNamedBoolean(L"finished"))
                {
                    m_finished = winrt::Windows::UI::Xaml::Visibility::Visible;
                }
                else
                {
                    m_finished = winrt::Windows::UI::Xaml::Visibility::Collapsed;
                }
                Finished().Visibility(m_finished);
                m_updatedAt = json.GetNamedString(L"updated_at");
                m_createdAt = json.GetNamedString(L"created_at");
                CreateDate().Text(m_createdAt);
                m_allowDownload = json.GetNamedBoolean(L"allowDownload");
                m_allowComment = json.GetNamedBoolean(L"allowComment");
                Comments().IsEnabled(m_allowComment);
                if (!m_allowComment) {
                    auto resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() };
                    CommentsTip().Content(box_value(resourceLoader.GetString(L"FailComment")));
                }
                m_totalLikes = json.GetNamedNumber(L"totalLikes");
                m_totalViews = json.GetNamedNumber(L"totalViews");
                m_viewsCount = json.GetNamedNumber(L"viewsCount");
                m_likesCount = json.GetNamedNumber(L"likesCount");
                LikeCounts().Value(m_likesCount);
                LikesCount().Text(to_hstring(m_likesCount));
                TotalViews().Text(to_hstring(m_viewsCount));
                m_isFavourite = json.GetNamedBoolean(L"isFavourite");
                Favourite().IsChecked(m_isFavourite);
                m_isLiked = json.GetNamedBoolean(L"isLiked");
                Like().IsChecked(m_isLiked);
                m_commentsCount = json.GetNamedNumber(L"commentsCount");
                CommentCounts().Value(m_commentsCount);
                co_await Eps(1);
            }
            else if (code == (double)401 && resp.GetNamedString(L"error") == L"1005")
            {

            }
            else
            {

            }

        }
        Windows::ApplicationModel::Resources::ResourceLoader resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() };
        if (Favourite().IsChecked().GetBoolean())
        {
            FavouriteTip().Content(box_value(resourceLoader.GetString(L"TipUnfavourite")));
        }
        else
        {
            FavouriteTip().Content(box_value(resourceLoader.GetString(L"TipFavourite")));
        }
        CommentsTip().Content(box_value(resourceLoader.GetString(L"TipComment")));
        if (Like().IsChecked().GetBoolean())
        {
            LikeTip().Content(box_value(resourceLoader.GetString(L"TipUnlike")));
        }
        else
        {
            LikeTip().Content(box_value(resourceLoader.GetString(L"TipLike")));
        }
    }
    
    winrt::Windows::Foundation::Collections::IObservableVector<bikabika::TagBlock> InfoPage::Tags()
    {
        return m_tags;
    }
    winrt::Windows::Foundation::Collections::IObservableVector<bikabika::EpisodeBlock> InfoPage::Episodes()
    {
        return m_eps;
    }

    void winrt::bikabika::implementation::InfoPage::MainGrid_SizeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::SizeChangedEventArgs const& e)
    {
        auto width = sender.as<Controls::Grid>().ActualWidth();
        if (width - 250 > 10) {
            Title().Width(width - 250);
            CategoriesString().Width(width - 250);
            Description().Width(width - 250);
        }
    }
    void winrt::bikabika::implementation::InfoPage::CreaterBorder_PointerPressed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
    {
        ToggleThemeTeachingTip1().Title(m_creater.Role());
        ToggleThemeTeachingTip1().IsOpen(true);
    }

    void winrt::bikabika::implementation::InfoPage::ListV_ItemClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::ItemClickEventArgs const& e)
    {
        auto epi = e.ClickedItem().as<bikabika::EpisodeBlock>();
        Frame().Navigate(winrt::xaml_typename<bikabika::PicPage>(), box_value(single_threaded_vector<winrt::Windows::Foundation::IInspectable>({ box_value(epi.BookId()),box_value(epi.ID()),box_value(epi.Order()),box_value(epi.Total()) })));
    }

    void winrt::bikabika::implementation::InfoPage::Comments_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        auto f = sender.as<winrt::Windows::UI::Xaml::Controls::AppBarToggleButton>().IsChecked().GetBoolean();
        CommentsView().IsPaneOpen(f);

    }


    void winrt::bikabika::implementation::InfoPage::CommentsView_PaneClosed(winrt::Windows::UI::Xaml::Controls::SplitView const& sender, winrt::Windows::Foundation::IInspectable const& args)
    {
        Comments().IsChecked(false);
        Windows::ApplicationModel::Resources::ResourceLoader resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() };
    }


    Windows::Foundation::IAsyncAction winrt::bikabika::implementation::InfoPage::Like_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        Like().IsChecked(!Like().IsChecked().GetBoolean());
        auto res{ co_await m_bikaHttp.Like(m_id) };
        if (res[1] == 'T')
        {
            ContentDialogShow(L"Timeout", L"");
        }
        else if (res[1] == 'E') {
            ContentDialogShow(L"Error", res);
        }
        else
        {
            Windows::Data::Json::JsonObject resp = Windows::Data::Json::JsonObject::Parse(res);
            double code = resp.GetNamedNumber(L"code");
            if (code == (double)200)
            {
                Windows::ApplicationModel::Resources::ResourceLoader resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() };
                Windows::Data::Json::JsonObject ca = resp.GetNamedObject(L"data");
                hstring action = ca.GetNamedString(L"action");
                if (action == L"like")
                {
                    Like().IsChecked(true);
                    LikeCounts().Value(LikeCounts().Value() + 1);
                }
                else
                {
                    Like().IsChecked(false);
                    LikeCounts().Value(LikeCounts().Value() - 1);
                }
            }
            //缺少鉴权
            else if (code == (double)401 && resp.GetNamedString(L"error") == L"1005")
            {
                ContentDialogShow(L"1005", L"");
            }
            //未知
            else
            {
                ContentDialogShow(L"Unknown", res);
            }
        }
    }


    Windows::Foundation::IAsyncAction winrt::bikabika::implementation::InfoPage::Favourite_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {

        Favourite().IsChecked(!Favourite().IsChecked().GetBoolean());

        auto res{ co_await m_bikaHttp.Favourite(m_id) };
        if (res[1] == 'T')
        {
            ContentDialogShow(L"Timeout", L"");
        }
        else if (res[1] == 'E') {
            ContentDialogShow(L"Error", res);
        }
        else
        {
            Windows::Data::Json::JsonObject resp = Windows::Data::Json::JsonObject::Parse(res);
            double code = resp.GetNamedNumber(L"code");
            if (code == (double)200)
            {
                Windows::ApplicationModel::Resources::ResourceLoader resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() };
                Windows::Data::Json::JsonObject ca = resp.GetNamedObject(L"data");
                hstring action = ca.GetNamedString(L"action");
                if (action == L"favourite")
                {
                    Favourite().IsChecked(true);
                }
                else
                {
                    Favourite().IsChecked(false);
                }
            }
            //缺少鉴权
            else if (code == (double)401 && resp.GetNamedString(L"error") == L"1005")
            {
                ContentDialogShow(L"1005", L"");
            }
            //未知
            else
            {
                ContentDialogShow(L"Unknown", res);
            }
        }
    }

}













// 爱心
void winrt::bikabika::implementation::InfoPage::Like_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    Windows::ApplicationModel::Resources::ResourceLoader resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() };
    if (sender.as<winrt::Windows::UI::Xaml::Controls::AppBarToggleButton>().IsChecked().GetBoolean())
    {
        LikeTip().Content(box_value(resourceLoader.GetString(L"TipUnlike")));
    }
    else
    {
        LikeTip().Content(box_value(resourceLoader.GetString(L"TipLike")));
    }
}

// 收藏
void winrt::bikabika::implementation::InfoPage::Favourite_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    Windows::ApplicationModel::Resources::ResourceLoader resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() };
    if (sender.as<winrt::Windows::UI::Xaml::Controls::AppBarToggleButton>().IsChecked().GetBoolean())
    {
        FavouriteTip().Content(box_value(resourceLoader.GetString(L"TipUnfavourite")));
    }
    else
    {
        FavouriteTip().Content(box_value(resourceLoader.GetString(L"TipFavourite")));
    }
}

// 点击标签
void winrt::bikabika::implementation::InfoPage::TagButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    auto tag = sender.as<winrt::Windows::UI::Xaml::Controls::Button>().Content().as<hstring>();
    extern bool loadComicFlag;
    loadComicFlag = true;
    ComicArgs args;
    args.ComicType(ComicsType::SEARCH);
    args.Content(tag);
    args.SortMode(winrt::bikabika::SearchSortMode::DD);
    args.IsAnime(false);
    Frame().Navigate(winrt::xaml_typename<bikabika::ComicsPage>(), box_value(args));
    
}



// 按下作者
void winrt::bikabika::implementation::InfoPage::Author_Drop(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    auto tag = sender.as<winrt::Windows::UI::Xaml::Controls::TextBlock>().Text();
    extern bool loadComicFlag;
    loadComicFlag = true;
    ComicArgs args;
    args.ComicType(ComicsType::SEARCH);
    args.Content(tag);
    args.SortMode(winrt::bikabika::SearchSortMode::DD);
    args.IsAnime(false);
    Frame().Navigate(winrt::xaml_typename<bikabika::ComicsPage>(), box_value(args));

}

// 按下标题
void winrt::bikabika::implementation::InfoPage::Title_PointerPressed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    //todo 点击标题复制
    auto title = sender.as<winrt::Windows::UI::Xaml::Controls::TextBlock>().Text();
   
    
}
