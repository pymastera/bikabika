﻿#include "pch.h"
#include "InfoPage.h"
#if __has_include("InfoPage.g.cpp")
#include "InfoPage.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::Data::Json;
using namespace Windows::UI::Input;

namespace winrt::bikabika::implementation
{
    InfoPage::InfoPage()
    {
        InitializeComponent();
    }
    winrt::Windows::Foundation::Collections::IObservableVector<BikaClient::Blocks::EpisodeBlock> InfoPage::Episodes()
    {
        return m_eps;
    }
    winrt::Windows::Foundation::Collections::IObservableVector<BikaClient::Blocks::ComicBlock> InfoPage::Comics()
    {
        return m_comics;
    }


    Windows::Foundation::IAsyncAction InfoPage::Comment(int32_t const& page)
    {
        auto res = co_await rootPage.HttpClient().Comments(m_id, page);
        if (res.Code() == -1)
        {
            rootPage.ContentDialogShow(BikaHttpStatus::TIMEOUT, L"");
        }
        else if (res.Code() == 200)
        {
            if (!m_isTop && res.TopComments().Size() > 0)
            {
                m_isTop = true;
                for (auto x : res.TopComments())
                {
                    m_comments.Append(x);
                }
            }
            for (auto y : res.Comments())
            {
                m_comments.Append(y);
            }
            BookComments().ItemsSource(box_value(m_comments));
        }
        else if (res.Code() == 401 && res.Error() == L"1005")
        {
            rootPage.ContentDialogShow(BikaHttpStatus::NOAUTH, res.Message());
        }
        else
        {
            rootPage.ContentDialogShow(BikaHttpStatus::UNKNOWN, res.Message());
        }
    }
    Windows::Foundation::IAsyncOperation<uint32_t> InfoPage::Eps(int32_t const& page)
    {
        auto res = co_await rootPage.HttpClient().Episodes(m_id, page);
        if (res.Code() == -1)
        {
            rootPage.ContentDialogShow(BikaHttpStatus::TIMEOUT, L"");
        }
        else if (res.Code() == 200)
        {
            for (auto x : res.Episodes())
            {
                m_eps.Append(x);
            }
            co_return res.Pages();
        }
        else if (res.Code() == 401 && res.Error() == L"1005")
        {
            rootPage.ContentDialogShow(BikaHttpStatus::NOAUTH, res.Message());
        }
        else
        {
            rootPage.ContentDialogShow(BikaHttpStatus::UNKNOWN, res.Message());
        }
        co_return 0;
    }

    winrt::Windows::Foundation::Collections::IObservableVector<BikaClient::Blocks::TagBlock> InfoPage::Tags()
    {
        return m_tags;
    }
    winrt::event_token InfoPage::PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        return m_propertyChanged.add(handler);
    }
    void InfoPage::PropertyChanged(winrt::event_token const& token) noexcept
    {
        m_propertyChanged.remove(token);
    }
    BikaClient::Blocks::BookBlock InfoPage::Book()
    {
        return m_book;
    }
    void InfoPage::Book(BikaClient::Blocks::BookBlock const& value)
    {
        m_book = value;
        m_propertyChanged(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"Book" });
    }
    Windows::Foundation::IAsyncAction InfoPage::Recommend()
    {
        auto res = co_await rootPage.HttpClient().Recommend(m_id);
        if (res.Code() == -1)
        {
            rootPage.ContentDialogShow(BikaHttpStatus::TIMEOUT, L"");
        }
        else if (res.Code() == 200)
        {

            for (auto x : res.Comics())
            {
                m_comics.Append(x);
            }


        }
        else if (res.Code() == 401 && res.Error() == L"1005")
        {
            rootPage.ContentDialogShow(BikaHttpStatus::NOAUTH, res.Message());
        }
        else
        {
            rootPage.ContentDialogShow(BikaHttpStatus::UNKNOWN, res.Message());
        }
    }
    Windows::Foundation::IAsyncAction InfoPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e)
    {
        auto params = winrt::unbox_value<winrt::Windows::Foundation::Collections::IVector<winrt::Windows::Foundation::IInspectable>>(e.Parameter());
        auto img = winrt::unbox_value<winrt::Windows::UI::Xaml::Media::ImageSource>(params.GetAt(0));
        m_id = winrt::unbox_value<ComicArgs>(params.GetAt(1)).BookId();
        __super::OnNavigatedTo(e);
        auto anim = winrt::Windows::UI::Xaml::Media::Animation::ConnectedAnimationService::GetForCurrentView().GetAnimation(L"ForwardConnectedAnimation");
        if (anim)
        {
            Img().Source(img);
            anim.TryStart(Img());
        }
        auto res = co_await rootPage.HttpClient().BookInfo(m_id);
        if (res.Code() == -1)
        {
            rootPage.ContentDialogShow(BikaHttpStatus::TIMEOUT, L"");
        }
        else if (res.Code() == 200)
        {
            auto book = res.BookInfos();
            Book(book);
            for (auto x : book.Tags())
            {
                m_tags.Append(x);
            }
            auto pages = co_await Eps(1);
            for (uint32_t i = 2; i <= pages; i++)
            {
                co_await Eps(i);
            }
            co_await Recommend();
            co_await Comment(1);
        }
        else if (res.Code() == 401 && res.Error() == L"1005")
        {
            rootPage.ContentDialogShow(BikaHttpStatus::NOAUTH, res.Message());
        }
        else
        {
            rootPage.ContentDialogShow(BikaHttpStatus::UNKNOWN, res.Message());
        }
    }

}
Windows::Foundation::IAsyncAction winrt::bikabika::implementation::InfoPage::Like_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

    auto res = co_await rootPage.HttpClient().Like(m_id);
    if (res.Code() == 200)
    {
        if (m_book.IsLiked())
        {
            rootPage.InfoBarMessageShow(resourceLoader.GetString(L"BlockLike/Text"), resourceLoader.GetString(L"Message/Book/Success"), Microsoft::UI::Xaml::Controls::InfoBarSeverity::Success);
        }
        else
        {
            rootPage.InfoBarMessageShow(resourceLoader.GetString(L"Message/Like/Cancel"), resourceLoader.GetString(L"Message/Book/Success"), Microsoft::UI::Xaml::Controls::InfoBarSeverity::Success);
        }
    }
    else
    {
        m_book.IsLiked(!sender.as<ToggleButton>().IsChecked().Value());
        if (m_book.IsLiked())
        {
            rootPage.InfoBarMessageShow(resourceLoader.GetString(L"BlockLike/Text"), resourceLoader.GetString(L"Message/Book/Fail"), Microsoft::UI::Xaml::Controls::InfoBarSeverity::Error);
        }
        else
        {
            rootPage.InfoBarMessageShow(resourceLoader.GetString(L"Message/Like/Cancel"), resourceLoader.GetString(L"Message/Book/Fail"), Microsoft::UI::Xaml::Controls::InfoBarSeverity::Error);
        }
    }
}
Windows::Foundation::IAsyncAction winrt::bikabika::implementation::InfoPage::Favourite_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

    auto res = co_await rootPage.HttpClient().Favourite(m_id);
    if (res.Code() == 200)
    {
        if (m_book.IsFavourite())
        {
            rootPage.InfoBarMessageShow(resourceLoader.GetString(L"BlockFavourite/Text"), resourceLoader.GetString(L"Message/Book/Success"), Microsoft::UI::Xaml::Controls::InfoBarSeverity::Success);
        }
        else
        {
            rootPage.InfoBarMessageShow(resourceLoader.GetString(L"Message/Favourite/Cancel"), resourceLoader.GetString(L"Message/Book/Success"), Microsoft::UI::Xaml::Controls::InfoBarSeverity::Success);
        }
    }
    else {
        m_book.IsFavourite(!sender.as<ToggleButton>().IsChecked().Value());
        if (m_book.IsFavourite())
        {
            rootPage.InfoBarMessageShow(resourceLoader.GetString(L"BlockFavourite/Text"), resourceLoader.GetString(L"Message/Book/Fail"), Microsoft::UI::Xaml::Controls::InfoBarSeverity::Error);
        }
        else
        {
            rootPage.InfoBarMessageShow(resourceLoader.GetString(L"Message/Favourite/Cancel"), resourceLoader.GetString(L"Message/Book/Fail"), Microsoft::UI::Xaml::Controls::InfoBarSeverity::Error);
        }
    }

}
void winrt::bikabika::implementation::InfoPage::TagButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    auto text = sender.as<Button>().Content().as<hstring>();
    winrt::Microsoft::UI::Xaml::Controls::SymbolIconSource symbol;
    symbol.Symbol(Symbol::Find);
    winrt::Windows::UI::Xaml::Controls::Frame frame;
    frame.Navigate(winrt::xaml_typename<bikabika::ComicPage>(), box_value(ComicArgs{ ComicsType::SEARCH, text, BikaClient::Utils::BikaSort{ BikaClient::Utils::SortMode::DD } }));
    rootPage.CreateNewTab(frame, text, symbol);
}
void winrt::bikabika::implementation::InfoPage::Author_Drop(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{

}
void winrt::bikabika::implementation::InfoPage::Title_PointerPressed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{

}
void winrt::bikabika::implementation::InfoPage::UsersName_PointerPressed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    PointerPoint currentPoint = e.GetCurrentPoint(NULL);
    PointerPointProperties props = currentPoint.Properties();
    if (props.IsRightButtonPressed()) {

    }
    else
    {
        auto text = sender.as<TextBlock>().Text();
        winrt::Microsoft::UI::Xaml::Controls::SymbolIconSource symbol;
        symbol.Symbol(Symbol::Find);
        winrt::Windows::UI::Xaml::Controls::Frame frame;
        frame.Navigate(winrt::xaml_typename<bikabika::ComicPage>(), box_value(ComicArgs{ ComicsType::SEARCH, text, BikaClient::Utils::BikaSort{ BikaClient::Utils::SortMode::DD } }));
        rootPage.CreateNewTab(frame, text, symbol);
    }
}
void winrt::bikabika::implementation::InfoPage::CommentFlyout_Closed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e)
{

}
void winrt::bikabika::implementation::InfoPage::CommentSubmit_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

}
void winrt::bikabika::implementation::InfoPage::MainGrid_SizeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::SizeChangedEventArgs const& e)
{

}
void winrt::bikabika::implementation::InfoPage::CreaterBorder_PointerPressed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    ToggleThemeTeachingTip1().Title(m_book.Creater().Role());
    ToggleThemeTeachingTip1().Target(sender.as<FrameworkElement>());
    ToggleThemeTeachingTip1().IsOpen(true);
}
void winrt::bikabika::implementation::InfoPage::ListV_ItemClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::ItemClickEventArgs const& e)
{

}
void winrt::bikabika::implementation::InfoPage::GridV_ItemClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::ItemClickEventArgs const& e)
{
    auto comicBlock = e.ClickedItem().as<BikaClient::Blocks::ComicBlock>();
    auto container = GridV().ContainerFromItem(e.ClickedItem()).as<winrt::Windows::UI::Xaml::Controls::GridViewItem>();
    auto root = container.ContentTemplateRoot().as<FrameworkElement>();
    auto image = root.FindName(L"ConnectedElement2").as<UIElement>();
    winrt::Windows::UI::Xaml::Media::Animation::ConnectedAnimationService::GetForCurrentView().PrepareToAnimate(L"ForwardConnectedAnimation", image);
    winrt::Microsoft::UI::Xaml::Controls::SymbolIconSource symbol;
    winrt::Windows::UI::Xaml::Controls::Frame frame;
    symbol.Symbol(Windows::UI::Xaml::Controls::Symbol::PreviewLink);
    frame.Navigate(winrt::xaml_typename<bikabika::InfoPage>(), box_value(single_threaded_vector<winrt::Windows::Foundation::IInspectable>({ box_value(root.FindName(L"ConnectedElement2").as<winrt::Windows::UI::Xaml::Controls::Image>().Source()), box_value(bikabika::ComicArgs(comicBlock.ID(),1,1,1)) })), winrt::Windows::UI::Xaml::Media::Animation::SuppressNavigationTransitionInfo());
    rootPage.CreateNewTab(frame, comicBlock.Title(), symbol);
}

void winrt::bikabika::implementation::InfoPage::Button_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    Windows::Storage::ApplicationDataContainer historys = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"Historys", Windows::Storage::ApplicationDataCreateDisposition::Always);
    hstring text = sender.as<Button>().Content().as<hstring>();
    if (historys.Values().HasKey(L"Search"))
    {
        uint32_t i;
        bool f = false;
        JsonArray searchHistorys = Windows::Data::Json::JsonArray::Parse(historys.Values().Lookup(L"Search").as<hstring>());
        for (i = 0; i < searchHistorys.Size(); i++)
        {
            if (text == searchHistorys.GetStringAt(i))
            {
                f = true;
                break;
            }
        }
        if (f)
        {
            searchHistorys.RemoveAt(i);
        }
        searchHistorys.InsertAt(0, Windows::Data::Json::JsonValue::CreateStringValue(text));
        historys.Values().Insert(L"Search", box_value(searchHistorys.Stringify()));
    }
    else {
        Windows::Data::Json::JsonArray json;
        json.InsertAt(0, Windows::Data::Json::JsonValue::CreateStringValue(text));
        historys.Values().Insert(L"Search", box_value(json.Stringify()));
    }
}


void winrt::bikabika::implementation::InfoPage::CommentLike_PointerEntered(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    sender.as<Border>().Background(Media::SolidColorBrush{ Windows::UI::Colors::LightGray() });
}


void winrt::bikabika::implementation::InfoPage::CommentLike_PointerExited(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    sender.as<Border>().Background(Media::SolidColorBrush{ Windows::UI::Colors::Transparent() });
}
