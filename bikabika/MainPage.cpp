﻿/*****************************************************************//**
 * \file   MainPage.cpp
 * \brief  主页面
 *
 * \author kulujun
 * \date   March 2022
 *********************************************************************/
#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"
using namespace winrt::BikaClient::Blocks;
using namespace winrt::Windows::UI::Xaml::Media::Animation;
using namespace winrt::Windows::UI::Input;


using namespace std;
using namespace winrt;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Composition;

using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Hosting;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;

using namespace winrt::Windows::UI::Xaml::Media;
using namespace Windows::Foundation::Numerics;
using namespace Windows::Foundation::Collections;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::UI::ViewManagement;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;

namespace winrt::bikabika::implementation
{
	bikabika::MainPage MainPage::current{ nullptr };

	MainPage::MainPage()
	{

		InitializeComponent();
		MainPage::current = *this;

	}
	void MainPage::LoginViewShow(bool const& isOpen)
	{
		Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, [isOpen, this]()
			{
				LoginTeachingTip().IsOpen(isOpen);
			});
	}


	void MainPage::CreateNewTab(Windows::UI::Xaml::Controls::Frame const& frame, hstring const& title, Microsoft::UI::Xaml::Controls::SymbolIconSource const& symbol)
	{

		winrt::Microsoft::UI::Xaml::Controls::TabViewItem newItem;
		newItem.Header(box_value(title));
		newItem.IconSource(symbol);
		frame.MinHeight(MainRootGrid().ActualHeight() - 40);
		newItem.Content(frame);
		Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, [newItem, this]()
			{
				ContentTabView().TabItems().Append(newItem);
				ContentTabView().SelectedItem(newItem);
			});

	}

	void  MainPage::ContentDialogShow(bikabika::BikaHttpStatus const& mode, hstring const& message)
	{

		Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, [mode, message, this]()
			{
				TextBlock content;
				ContentDialog dialog;
				dialog.CloseButtonText(resourceLoader.GetString(L"FailMessage/CloseButton/Normal"));
				dialog.IsTextScaleFactorEnabled(true);
				StackPanel grid;
				grid.Orientation(Orientation::Vertical);
				Border boder;
				boder.CornerRadius(Windows::UI::Xaml::CornerRadius{ 10,10,10,10 });
				boder.Padding(Thickness{ 10,5,10,5 });
				boder.VerticalAlignment(VerticalAlignment::Top);
				boder.HorizontalAlignment(HorizontalAlignment::Left);
				dialog.RequestedTheme(Window::Current().Content().as<FrameworkElement>().RequestedTheme());
				TextBlock title;
				Image img;
				img.Height(270);
				img.Width(270);
				img.VerticalAlignment(VerticalAlignment::Center);
				img.HorizontalAlignment(HorizontalAlignment::Center);
				title.FontWeight(Text::FontWeights::Bold());
				content.HorizontalAlignment(HorizontalAlignment::Center);
				if (mode == bikabika::BikaHttpStatus::TIMEOUT) {
					title.Text(resourceLoader.GetString(L"FailMessage/Title/TimeOut"));
					img.Source(BitmapImage{ Uri{ L"ms-appx:///Assets//Picacgs//icon_unknown_error.png" } });
					StackPanel stackPanel;
					stackPanel.Orientation(Orientation::Vertical);
					stackPanel.HorizontalAlignment(HorizontalAlignment::Center);
					TextBlock content1, content2, content3, content4;
					content1.FontWeight(Text::FontWeights::Bold());
					content1.HorizontalAlignment(HorizontalAlignment::Center);
					content2.FontWeight(Text::FontWeights::Bold());
					content2.HorizontalAlignment(HorizontalAlignment::Center);
					content3.FontWeight(Text::FontWeights::Bold());
					content3.HorizontalAlignment(HorizontalAlignment::Center);
					content4.FontWeight(Text::FontWeights::Bold());
					content4.HorizontalAlignment(HorizontalAlignment::Center);
					content1.Text(resourceLoader.GetString(L"FailMessage/Message/TimeOut/One"));
					content3.Text(resourceLoader.GetString(L"FailMessage/Message/TimeOut/Two"));
					content4.Text(resourceLoader.GetString(L"FailMessage/Message/TimeOut/Three"));
					stackPanel.Children().Append(content1);
					stackPanel.Children().Append(content2);
					stackPanel.Children().Append(content3);
					stackPanel.Children().Append(content4);
					dialog.Content(box_value(stackPanel));
				}
				else if (mode == bikabika::BikaHttpStatus::NOAUTH)
				{
					title.Text(resourceLoader.GetString(L"FailMessage/Message/NoAuth"));
					img.Source(BitmapImage{ Uri{ L"ms-appx:///Assets//Picacgs//icon_exclamation_error.png" } });
					dialog.PrimaryButtonText(resourceLoader.GetString(L"FailMessage/PrimaryButton/NoAuth"));
					dialog.PrimaryButtonClick({ [this](ContentDialog const&, ContentDialogButtonClickEventArgs const&) {LoginViewShow(true); } });
					dialog.DefaultButton(ContentDialogButton::Primary);
					dialog.IsPrimaryButtonEnabled(true);
					content.Text(message);
					dialog.Content(content);
				}
				else if (mode == bikabika::BikaHttpStatus::BLANK)
				{
					title.Text(resourceLoader.GetString(L"FailMessage/Title/Write"));
					img.Source(BitmapImage{ Uri{ L"ms-appx:///Assets//Picacgs//icon_exclamation_error.png" } });
					content.Text(resourceLoader.GetString(L"FailMessage/Message/Blank"));
					dialog.Content(content);
				}
				else if (mode == bikabika::BikaHttpStatus::SUREERROR)
				{
					title.Text(resourceLoader.GetString(L"FailMessage/Title/Write"));
					img.Source(BitmapImage{ Uri{ L"ms-appx:///Assets//Picacgs//icon_exclamation_error.png" } });
					content.Text(resourceLoader.GetString(L"FailMessage/Message/PasswordSure"));
					dialog.Content(content);
				}
				else if (mode == bikabika::BikaHttpStatus::OLDPASSWORDERROR)
				{
					title.Text(resourceLoader.GetString(L"FailMessage/Title/OldPassWordError"));
					img.Source(BitmapImage{ Uri{ L"ms-appx:///Assets//Picacgs//icon_exclamation_error.png" } });
					content.Text(resourceLoader.GetString(L"FailMessage/Title/OldPassWordError"));
					dialog.Content(content);
				}
				else if (mode == bikabika::BikaHttpStatus::PASSWORDTOOSHORT)
				{
					title.Text(resourceLoader.GetString(L"FailMessage/Title/PassWordTooShort"));
					img.Source(BitmapImage{ Uri{ L"ms-appx:///Assets//Picacgs//icon_exclamation_error.png" } });
					content.Text(resourceLoader.GetString(L"FailMessage/Message/PassWordTooShort"));
					dialog.Content(content);
				}
				else
				{
					title.Text(resourceLoader.GetString(L"FailMessage/Title/Unknown"));
					img.Source(BitmapImage{ Uri{ L"ms-appx:///Assets//Picacgs//icon_exclamation_error.png" } });
					content.Text(message);
					dialog.Content(content);
				}
				boder.Child(title);
				grid.Children().Append(boder);
				grid.Children().Append(img);
				dialog.Title(box_value(grid));
				dialog.ShowAsync();
			});
	}

	void MainPage::LayoutMessageShow(hstring const& message, bool const& isOpen)
	{
		Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, [message, isOpen, this]()
			{
				LayoutMessage().Title(message);
				LayoutMessage().IsOpen(isOpen);
			});
	}
	void MainPage::LayoutMessageShow(bool const& isOpen)
	{
		Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, [isOpen, this]()
			{
				LayoutMessage().IsOpen(isOpen);
			});
	}
	winrt::event_token MainPage::PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
	{
		return m_propertyChanged.add(handler);
	}
	void MainPage::PropertyChanged(winrt::event_token const& token) noexcept
	{
		m_propertyChanged.remove(token);
	}
	bool MainPage::IsLogin()
	{
		return m_login;
	}
	void MainPage::IsLogin(bool const& value)
	{
		m_login = value;
	}

	winrt::Windows::Foundation::Collections::IObservableVector<BikaClient::Blocks::UserBlock> MainPage::LoginUsers()
	{
		return m_loginUsers;
	}

	Windows::Foundation::IAsyncAction MainPage::Login()
	{
		m_login = false;
		auto res = co_await m_bikaClient.Login(Email().Text(), Password().Password());
		if (res.Code()== -1)
		{
			LoginButton().Content(box_value(resourceLoader.GetString(L"ButtonLogin/Content")));
			LoginButton().IsEnabled(true);
			ContentDialogShow(BikaHttpStatus::TIMEOUT, L"");
		}
		else if (res.Code() == 200)
		{
			FlyoutSwitchAccounts().Items().Clear();
			m_login = true;
			Windows::Storage::ApplicationDataContainer loginData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"LoginData", Windows::Storage::ApplicationDataCreateDisposition::Always);
			JsonArray emailArray = m_emails.GetNamedArray(L"emailArray");
			OutputDebugStringW(emailArray.Stringify().c_str());
			m_emails.SetNamedValue(L"Last", JsonValue::CreateStringValue(Email().Text()));
			bool f = true;
			for (auto x : emailArray)
			{
				if (x.GetString() == Email().Text())
				{
					f = false;
					break;
				}
			}
			if(f) emailArray.Append(JsonValue::CreateStringValue(Email().Text()));
			m_emails.SetNamedValue(L"emailArray", emailArray);
			loginData.Values().Insert(L"emails", box_value(m_emails.Stringify()));
			co_await SetPerson();
			InfoBarMessageShow(resourceLoader.GetString(L"Message/Login/Success"), m_user.Name(), Microsoft::UI::Xaml::Controls::InfoBarSeverity::Success);
			if (RememberCheckBox().IsChecked().GetBoolean())
			{
				m_passwords.SetNamedValue(Email().Text(), JsonValue::CreateStringValue(Password().Password()));
				loginData.Values().Insert(L"passwords", box_value(m_passwords.Stringify()));
			}
			if(!m_user.IsPunched()) {
				co_await PunchIn();
				co_await SetPerson();
			}
			if (emailArray.Size() > 1)
			{
				FlyoutSwitchAccounts().IsEnabled(true);
				for (auto x : emailArray)
				{
					if (m_passwords.HasKey(x.GetString()) && m_userDatas.HasKey(x.GetString()) && x.GetString() != m_user.Email())
					{
						MenuFlyoutItem newItem;
						UserBlock userblock = UserBlock(m_userDatas.GetNamedObject(x.GetString()));
						newItem.Text(userblock.Name());
						newItem.Icon(SymbolIcon{Symbol::People});
						newItem.Click([this](Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args)
							{
								auto email = m_userDatas.GetNamedString(sender.as<MenuFlyoutItem>().Text());
								Email().Text(email);
								Password().Password(m_passwords.GetNamedString(email));
								//LoginViewShow(true);
								InfoBarMessageShow(resourceLoader.GetString(L"Message/Login/Switch"), sender.as<MenuFlyoutItem>().Text(), Microsoft::UI::Xaml::Controls::InfoBarSeverity::Informational);
								auto login{ Login() };
							});

						FlyoutSwitchAccounts().Items().Append(newItem);
					}
				}
			}
			else
			{
				FlyoutSwitchAccounts().IsEnabled(false);
			}
		}
		else if(res.Code()==401&&res.Error()==L"1005")
		{

			ContentDialogShow(BikaHttpStatus::NOAUTH, res.Message());
		}
		else if (res.Code() == 400)
		{

			ContentDialogShow(BikaHttpStatus::UNKNOWN, resourceLoader.GetString(L"FailMessage/Message/Login/Error"));
			Password().Password(L"");
			m_passwords.Remove(Email().Text());

			Windows::Storage::ApplicationDataContainer loginData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"LoginData", Windows::Storage::ApplicationDataCreateDisposition::Always);
			loginData.Values().Insert(L"passwords", box_value(m_passwords.Stringify()));
		}
		else
		{

			ContentDialogShow(BikaHttpStatus::UNKNOWN, res.Message());
			Password().Password(L"");
		}
		LoginButton().Content(box_value(resourceLoader.GetString(L"Keyword/Login")));
		LoginButton().IsEnabled(true);

	}

	Windows::Foundation::IAsyncAction MainPage::PunchIn()
	{
		auto res = co_await m_bikaClient.PunchIn();
		auto action = res.Action();

		if (action == BikaClient::Responses::Actions::OK)
		{
			InfoBarMessageShow(resourceLoader.GetString(L"Message/AutoPunchIn"), resourceLoader.GetString(L"Message/PunchIned"), Microsoft::UI::Xaml::Controls::InfoBarSeverity::Success);
		}
		else if (action == BikaClient::Responses::Actions::Fail)
		{
			InfoBarMessageShow(resourceLoader.GetString(L"Message/AutoPunchIn"), resourceLoader.GetString(L"Message/PunchIned"), Microsoft::UI::Xaml::Controls::InfoBarSeverity::Informational);
		}
		else
		{
			InfoBarMessageShow(resourceLoader.GetString(L"Message/AutoPunchIn"), res.Message(), Microsoft::UI::Xaml::Controls::InfoBarSeverity::Error);
		}
	}

	void MainPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e)
	{

		// 隐藏标题栏
		auto coreTitleBar = CoreApplication::GetCurrentView().TitleBar();
		coreTitleBar.ExtendViewIntoTitleBar(true);
		Window::Current().SetTitleBar(CustomDragRegion());

		auto titleBar = ApplicationView::GetForCurrentView().TitleBar();
		titleBar.ButtonBackgroundColor(Colors::Transparent());
		__super::OnNavigatedTo(e);
		// 登录初始化
		LogOut();
		NavHome().IsEnabled(false);
		NavClassification().IsEnabled(false);
		NavAccount().IsEnabled(false);

		Windows::Storage::ApplicationDataContainer loginData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"LoginData", Windows::Storage::ApplicationDataCreateDisposition::Always);

		if (loginData.Values().HasKey(L"emails")) {
			JsonObject::TryParse(loginData.Values().Lookup(L"emails").as<hstring>(), m_emails);
			OutputDebugStringW(m_emails.Stringify().c_str());
		}
		else
		{
			JsonArray emailArray;
			m_emails.Insert(L"emailArray", emailArray);
		}
		if(loginData.Values().HasKey(L"passwords"))
		{
			JsonObject::TryParse(loginData.Values().Lookup(L"passwords").as<hstring>(), m_passwords);
		}
		if (loginData.Values().HasKey(L"userData"))
		{
			JsonObject::TryParse(loginData.Values().Lookup(L"userData").as<hstring>(), m_userDatas);
		}
		OutputDebugStringW(m_emails.Stringify().c_str());


	}
	/// <summary>
	/// 把过长文字转换成带省略号
	/// </summary>
	/// <param name="str">原字符</param>
	/// <param name="length">限定长度</param>
	/// <returns>带省略号的字符</returns>
	hstring MainPage::Omit(hstring const& str, uint32_t const& length)
	{
		wstring text;
		uint32_t i = 0;
		for (auto a : str)
		{
			if (i == length) return hstring{ text + L" ..." };
			text += a;
			i++;
		}
		return str;
	}
	void MainPage::StartInfoBar()
	{
		timer.Interval(std::chrono::milliseconds{ 100 });
		auto registrationtoken = timer.Tick({ this, &MainPage::OnTick });
		timer.Start();
	}
	void MainPage::Register(bool const& isOpen)
	{
		Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, [isOpen, this]()
			{
				RegisterEmail().Text(L"");
				RegisterName().Text(L"");
				RegisterPassword().Password(L"");
				RegisterPasswordSure().Password(L"");
				Question1Box().Text(L"");
				Question2Box().Text(L"");
				Question3Box().Text(L"");
				Answer1Box().Text(L"");
				Answer2Box().Text(L"");
				Answer3Box().Text(L"");
				RegisterTeachingTip().IsOpen(isOpen);
			});
	}
	void MainPage::ChangePassword(bool const& isOpen)
	{
		Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, [isOpen,this]()
			{
				OldPasswrod().Text(L"");
				NewPasswrod().Text(L"");
				ChangePasswordTip().IsOpen(isOpen);

			});
	}
	void MainPage::ChangeTitle(bool const& isOpen)
	{
		Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, [isOpen, this]()
			{
				NewTitle().Text(L"");
				ChangeTitleTip().IsOpen(isOpen);

			});
	}
	void MainPage::ChangeSignature(bool const& isOpen)
	{
		Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, [isOpen, this]()
			{
				NewSignature().Text(L"");
				ChangeSignatureTip().IsOpen(isOpen);
			});
	}

	void MainPage::OnTick(Windows::Foundation::IInspectable const& /* sender */,Windows::Foundation::IInspectable const& /* e */)
	{

		if (InfoBar().Value() < 100)
		{
			InfoBar().Value(InfoBar().Value() + 3);
		}
		else
		{
			Info().IsOpen(false);
			timer.Stop();
		}
	}

	void MainPage::InfoBarMessageShow(hstring const& title, hstring const& message, winrt::Microsoft::UI::Xaml::Controls::InfoBarSeverity const& severity)
	{

		Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, [title, message, severity, this]()
			{
				if (timer.IsEnabled()) timer.Stop();
				timer = Windows::UI::Xaml::DispatcherTimer();
				Info().IsOpen(false);
				InfoBar().Value(0);
				Info().Message(message);
				Info().Title(title);
				Info().Severity(severity);
				Info().IsOpen(true);
				StartInfoBar();
				LayoutMessage().IsOpen(false);
			});
	}
	void MainPage::LogOut()
	{

		Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, [this]()
			{
				m_login = false;
				m_user = BikaClient::Blocks::UserBlock{ nullptr };
				BigUserImg().ProfilePicture(BitmapImage{ Uri{ L"ms-appx:///Assets//Picacgs//placeholder_avatar_2.png" } });
				SmallUserImg().ProfilePicture(BitmapImage{ Uri{ L"ms-appx:///Assets//Picacgs//placeholder_avatar_2.png" } });
				UserName().Text(resourceLoader.GetString(L"Keyword/Default/Name"));
				UserLevel().Text(resourceLoader.GetString(L"Keyword/Default/Level"));
				UserSlogan().Text(resourceLoader.GetString(L"Keyword/Default/Slogan"));
				LoginViewShow(true);
			});

	}

	BikaClient::Blocks::UserBlock MainPage::User()
	{
		return m_user;
	}
	void MainPage::User(BikaClient::Blocks::UserBlock const& value)
	{
		m_user = value;
		m_propertyChanged(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"User" });
	}
	Windows::Foundation::IAsyncAction MainPage::SetPerson()
	{
		auto res = co_await m_bikaClient.PersonInfo();
		LayoutMessageShow(false);
		LoginViewShow(false);
		if (res.Code() == -1)
		{
			ContentDialogShow(BikaHttpStatus::TIMEOUT, L"");
		}
		else if (res.Code() == 200)
		{
			Windows::Storage::ApplicationDataContainer loginData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"LoginData", Windows::Storage::ApplicationDataCreateDisposition::Always);
			NavHome().IsEnabled(true);
			NavClassification().IsEnabled(true);
			NavAccount().IsEnabled(true);
			User(res.User());
			m_userDatas.SetNamedValue(res.User().Name(), JsonValue::CreateStringValue(res.User().Email()));
			m_userDatas.SetNamedValue(res.User().Email(), JsonObject::Parse(res.User().Json()));
			OutputDebugStringW(m_emails.Stringify().c_str());
			loginData.Values().Insert(L"userData", box_value(m_userDatas.Stringify()));
			if (m_firstArrive) {
				winrt::Microsoft::UI::Xaml::Controls::SymbolIconSource symbol;
				symbol.Symbol(Symbol::Home);
				winrt::Windows::UI::Xaml::Controls::Frame frame;
				frame.Navigate(winrt::xaml_typename<bikabika::HomePage>());
				CreateNewTab(frame, resourceLoader.GetString(L"NavHome/Content"), symbol);
				NavView().SelectedItem(NavView().MenuItems().GetAt(5));
				m_firstArrive = false;
				co_await GetKeywords();
			}
		}
		else if (res.Code() == 401 && res.Error() == L"1005")
		{
			ContentDialogShow(BikaHttpStatus::NOAUTH, res.Message());
		}
		else
		{
			ContentDialogShow(BikaHttpStatus::UNKNOWN, res.Message());
		}
	}
	Windows::Foundation::IAsyncAction MainPage::GetKeywords()
	{
		auto res =  co_await m_bikaClient.Keywords() ;
		if (res.Code() == -1)
		{
			ContentDialogShow(BikaHttpStatus::TIMEOUT, L"");
		}
		else if (res.Code() == 200)
		{
			Windows::Storage::ApplicationDataContainer historys = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"Historys", Windows::Storage::ApplicationDataCreateDisposition::Always);
			if (historys.Values().HasKey(L"Search"))
			{
				auto y = Windows::Data::Json::JsonArray::Parse(historys.Values().Lookup(L"Search").as<hstring>());

				for (auto z : y)
				{
					m_suggestions.Append(winrt::make<KeywordBox>(z.GetString(), resourceLoader.GetString(L"Keyword/SearchHistory"), L"\xE81C"));
				}
			}
			for (auto x : res.Keywords())
			{
				m_suggestions.Append(winrt::make<bikabika::implementation::KeywordBox>(x.Tag(), resourceLoader.GetString(L"Keyword/Keywords"), L"\xE8EC"));
			}
		}
		else if (res.Code() == 401 && res.Error() == L"1005")
		{
			ContentDialogShow(BikaHttpStatus::NOAUTH, res.Message());

		}
		else if (res.Code() == 400)
		{
			ContentDialogShow(BikaHttpStatus::UNKNOWN, resourceLoader.GetString(L"FailMessage/Message/Login/Error"));
			Password().Password(L"");
		}
		else
		{
			ContentDialogShow(BikaHttpStatus::UNKNOWN, res.Message());
			Password().Password(L"");
		}

	}

	BikaClient::BikaHttpClient MainPage::HttpClient()
	{
		return m_bikaClient;
	}

}

void winrt::bikabika::implementation::MainPage::LoginClickHandler(Windows::Foundation::IInspectable const& /*sender*/, Windows::UI::Xaml::RoutedEventArgs const& /*args*/)
{
	// 账号密码为空
	if (Email().Text() == L"" || Password().Password() == L"")
	{
		ContentDialogShow(BikaHttpStatus::UNKNOWN, resourceLoader.GetString(L"FailMessage/Message/Login/Blank"));
	}
	else
	{
		Microsoft::UI::Xaml::Controls::ProgressRing ring;
		ring.IsActive(true);
		ring.Width(16);
		ring.Height(16);
		LoginButton().Content(ring);
		LoginButton().IsEnabled(false);
		auto login{ Login() };
	}
}
void winrt::bikabika::implementation::MainPage::ContentFrame_NavigationFailed(Windows::Foundation::IInspectable const&, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs const& /*args*/)
{
}
void winrt::bikabika::implementation::MainPage::NavView_ItemInvoked(Windows::Foundation::IInspectable const&, muxc::NavigationViewItemInvokedEventArgs const& args)
{
	if (args.IsSettingsInvoked())
	{
		NavView_Navigate(L"settings", args.RecommendedNavigationTransitionInfo());
	}
	else if (args.InvokedItemContainer())
	{
		NavView_Navigate(winrt::unbox_value_or<winrt::hstring>(args.InvokedItemContainer().Tag(), L"").c_str(), args.RecommendedNavigationTransitionInfo());
	}
}
void winrt::bikabika::implementation::MainPage::NavView_Navigate(std::wstring navItemTag, Windows::UI::Xaml::Media::Animation::NavigationTransitionInfo const& /*transitionInfo*/)
{
	winrt::Microsoft::UI::Xaml::Controls::SymbolIconSource symbol;
	winrt::Windows::UI::Xaml::Controls::Frame frame;
	for (auto item : ContentTabView().TabItems())
	{
		if (winrt::unbox_value_or<winrt::hstring>(item.as<winrt::Microsoft::UI::Xaml::Controls::TabViewItem>().Header(), L"") == resourceLoader.GetString(hstring{L"Header/" + navItemTag}))
		{
			ContentTabView().SelectedItem(item);
			return;
		}
	}
	if (navItemTag == L"settings")
	{
		symbol.Symbol(Symbol::Setting);
		frame.Navigate(winrt::xaml_typename<bikabika::SettingsPage>());
	}
	else if (navItemTag == L"Home")
	{
		frame.Navigate(winrt::xaml_typename<bikabika::HomePage>());
		symbol.Symbol(Symbol::Home);
	}
	else if (navItemTag == L"Classification")
	{
		symbol.Symbol(Symbol::AllApps);
		frame.Navigate(winrt::xaml_typename<bikabika::ClassificationPage>());
	}
	else if (navItemTag == L"User")
	{
		frame.Navigate(winrt::xaml_typename<bikabika::UserPage>());
		symbol.Symbol(Symbol::Contact);
	}
	else if (navItemTag == L"Download")
	{
		frame.Navigate(winrt::xaml_typename<bikabika::DownloadPage>());
		symbol.Symbol(Symbol::Download);
	}
	else return;
	CreateNewTab(frame, resourceLoader.GetString(hstring{ L"Header/" + navItemTag }) , symbol);
}
void winrt::bikabika::implementation::MainPage::UsersPic_PointerPressed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
	PointerPoint currentPoint = e.GetCurrentPoint(NULL);
	PointerPointProperties props = currentPoint.Properties();
	if (m_login) {
		if (props.IsRightButtonPressed()) {
			Menu().ShowAt(sender.as<FrameworkElement>());
		}
		else if (props.IsLeftButtonPressed())
		{
			NavView_Navigate(L"User", Windows::UI::Xaml::Media::Animation::EntranceNavigationTransitionInfo());
		}
	}
	else {
		LoginViewShow(true);
	}
}
void winrt::bikabika::implementation::MainPage::AutoCheckBox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& /*e*/)
{
	if (sender.as<CheckBox>().IsChecked().GetBoolean())
	{
		RememberCheckBox().IsChecked(true);
	}

	Windows::Storage::ApplicationDataContainer loginData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"LoginData", Windows::Storage::ApplicationDataCreateDisposition::Always);
	loginData.Values().Insert(L"AutoLogin", box_value(AutoCheckBox().IsChecked().GetBoolean()));
}
void winrt::bikabika::implementation::MainPage::RememberCheckBox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& /*e*/)
{
	if (!sender.as<CheckBox>().IsChecked().GetBoolean())
	{
		AutoCheckBox().IsChecked(false);
	}

	Windows::Storage::ApplicationDataContainer loginData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"LoginData", Windows::Storage::ApplicationDataCreateDisposition::Always);
	loginData.Values().Insert(L"RememberMe", box_value(RememberCheckBox().IsChecked().GetBoolean()));
}

void winrt::bikabika::implementation::MainPage::ContentTabView_TabCloseRequested(winrt::Microsoft::UI::Xaml::Controls::TabView const& sender, winrt::Microsoft::UI::Xaml::Controls::TabViewTabCloseRequestedEventArgs const& args)
{
	uint32_t tabIndexFromControl;
	if (sender.TabItems().IndexOf(args.Tab(), tabIndexFromControl))
	{
		sender.TabItems().RemoveAt(tabIndexFromControl);
	}

}
void winrt::bikabika::implementation::MainPage::Grid_SizeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::SizeChangedEventArgs const& /*e*/)
{
	ContentTabView().Width(sender.as<Controls::Frame>().ActualWidth());
}
void winrt::bikabika::implementation::MainPage::NavView_PaneClosed(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& /*sender*/, winrt::Windows::Foundation::IInspectable const& /*args*/)
{
	ContentFrame().Margin(Thickness{ 47,0,0,0 });
	APPTitle().Visibility(Visibility::Collapsed);
	NavSmallImg().Visibility(Visibility::Visible);
	NavBigImg().Visibility(Visibility::Collapsed);
}
void winrt::bikabika::implementation::MainPage::NavView_PaneOpened(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& /*sender*/, winrt::Windows::Foundation::IInspectable const& /*args*/)
{
	ContentFrame().Margin(Thickness{ 179,0,0,0 });
	APPTitle().Visibility(Visibility::Visible);
	NavSmallImg().Visibility(Visibility::Collapsed);
	NavBigImg().Visibility(Visibility::Visible);
}
void winrt::bikabika::implementation::MainPage::Flyout_Opened(winrt::Windows::Foundation::IInspectable const& /*sender*/, winrt::Windows::Foundation::IInspectable const& /*e*/)
{
	CatSearch().Text(L" ");
	CatSearch().ItemsSource(box_value(m_suggestions));
	CatSearch().Text(L"");
}
void winrt::bikabika::implementation::MainPage::LogOut_Click(winrt::Windows::Foundation::IInspectable const& /*sender*/, winrt::Windows::UI::Xaml::RoutedEventArgs const& /*e*/)
{
	ContentDialog dialog = ContentDialog();
	StackPanel stack;
	stack.Orientation(Orientation::Horizontal);
	stack.Children().Append(SymbolIcon{ Symbol::LeaveChat });
	TextBlock text;
	text.Text(resourceLoader.GetString(L"Keyword/Tip/Tip"));
	stack.Children().Append(text);
	dialog.Title(stack);
	dialog.Content(box_value(resourceLoader.GetString(L"Keyword/Tip/LogOut")));
	dialog.PrimaryButtonText(resourceLoader.GetString(L"Keyword/Sure"));
	dialog.SecondaryButtonText(resourceLoader.GetString(L"Keyword/Cancel"));
	dialog.DefaultButton(ContentDialogButton::Secondary);
	dialog.IsPrimaryButtonEnabled(true);
	dialog.IsSecondaryButtonEnabled(true);
	dialog.RequestedTheme(Window::Current().Content().as<FrameworkElement>().RequestedTheme());
	dialog.PrimaryButtonClick([this](ContentDialog const&, ContentDialogButtonClickEventArgs const&) {
		Windows::Storage::ApplicationDataContainer loginData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"LoginData", Windows::Storage::ApplicationDataCreateDisposition::Always);
		AutoCheckBox().IsChecked(false);
		LogOut();
		});
	dialog.SecondaryButtonClick([](ContentDialog const&, ContentDialogButtonClickEventArgs const&) {
		return;
		});
	dialog.ShowAsync();
}
void winrt::bikabika::implementation::MainPage::ChangeSignature_Click(winrt::Windows::Foundation::IInspectable const& /*sender*/, winrt::Windows::UI::Xaml::RoutedEventArgs const& /*e*/)
{
	ChangeSignature(true);
}
/// <summary>
/// 菜单栏中修改密码按钮
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void winrt::bikabika::implementation::MainPage::ChangePassword_Click(winrt::Windows::Foundation::IInspectable const& /*sender*/, winrt::Windows::UI::Xaml::RoutedEventArgs const& /*e*/)
{
	ChangePassword(true);
}
/// <summary>
/// 密码框按回车响应
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void winrt::bikabika::implementation::MainPage::LoginButton_KeyUp(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::KeyRoutedEventArgs const& e)
{
	if (e.Key() == Windows::System::VirtualKey::Enter)
	{
		LoginClickHandler(sender, Windows::UI::Xaml::RoutedEventArgs{ nullptr });
	}
}
/// <summary>
/// 修改签名框按回车响应
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void winrt::bikabika::implementation::MainPage::ChangePasswordButton_KeyUp(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::KeyRoutedEventArgs const& e)
{
	if (e.Key() == Windows::System::VirtualKey::Enter)
	{
		ChangePasswordButton_Click(sender, Windows::UI::Xaml::RoutedEventArgs{ nullptr });
	}
}
/// <summary>
/// 检测搜索框内容更改
/// </summary>
/// <param name="sender"></param>
/// <param name="args"></param>
void winrt::bikabika::implementation::MainPage::CatSearch_TextChanged(winrt::Windows::UI::Xaml::Controls::AutoSuggestBox const& sender, winrt::Windows::UI::Xaml::Controls::AutoSuggestBoxTextChangedEventArgs const& args)
{
	if (sender.Text() != L"" && (args.Reason() == AutoSuggestionBoxTextChangeReason::UserInput || (args.Reason() == AutoSuggestionBoxTextChangeReason::ProgrammaticChange && sender.Text() == L" ")))
	{
		if (!m_suggestIsChosen)
		{
			sender.ItemsSource(box_value(m_suggestions));
		}
		m_suggestIsChosen = false;
	}
}
/// <summary>
/// 搜索框提交
/// </summary>
/// <param name="sender"></param>
/// <param name="args"></param>
void winrt::bikabika::implementation::MainPage::CatSearch_QuerySubmitted(winrt::Windows::UI::Xaml::Controls::AutoSuggestBox const& sender, winrt::Windows::UI::Xaml::Controls::AutoSuggestBoxQuerySubmittedEventArgs const& /*args*/)
{

	Windows::Storage::ApplicationDataContainer historys = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"Historys", Windows::Storage::ApplicationDataCreateDisposition::Always);
	hstring text = sender.Text();
	if (text == L"") return;
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
			m_suggestions.RemoveAt(i);
		}
		searchHistorys.InsertAt(0, Windows::Data::Json::JsonValue::CreateStringValue(text));
		historys.Values().Insert(L"Search", box_value(searchHistorys.Stringify()));
		m_suggestions.InsertAt(0, winrt::make<KeywordBox>(text, resourceLoader.GetString(L"Keyword/SearchHistory"), L"\xE81C"));
	}
	else {
		Windows::Data::Json::JsonArray json;
		json.InsertAt(0, Windows::Data::Json::JsonValue::CreateStringValue(text));
		historys.Values().Insert(L"Search", box_value(json.Stringify()));
		m_suggestions.InsertAt(0, winrt::make<KeywordBox>(text, resourceLoader.GetString(L"Keyword/SearchHistory"), L"\xE81C"));
	}
	SearchFlyout().Hide();
	winrt::Microsoft::UI::Xaml::Controls::SymbolIconSource symbol;
	symbol.Symbol(Symbol::Find);
	winrt::Windows::UI::Xaml::Controls::Frame frame;
	frame.Navigate(winrt::xaml_typename<bikabika::ComicPage>(), box_value(ComicArgs{ ComicsType::SEARCH, text, BikaClient::Utils::BikaSort{ BikaClient::Utils::SortMode::DD } }));
	CreateNewTab(frame, text, symbol);
}
/// <summary>
/// 搜索框建议选择
/// </summary>
/// <param name="sender"></param>
/// <param name="args"></param>
void winrt::bikabika::implementation::MainPage::CatSearch_SuggestionChosen(winrt::Windows::UI::Xaml::Controls::AutoSuggestBox const& sender, winrt::Windows::UI::Xaml::Controls::AutoSuggestBoxSuggestionChosenEventArgs const& args)
{
	m_suggestIsChosen = true;
	hstring s = args.SelectedItem().as<bikabika::KeywordBox>().Keyword();
	sender.Text(s);
}
/// <summary>
/// 邮箱与密码与自动登录控制
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void winrt::bikabika::implementation::MainPage::Password_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& /*e*/)
{
	Windows::Storage::ApplicationDataContainer settings = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"Settings", Windows::Storage::ApplicationDataCreateDisposition::Always);

	LoginButton().Content(box_value(resourceLoader.GetString(L"Keyword/Login")));
	LoginButton().IsEnabled(true);

	Windows::Storage::ApplicationDataContainer loginData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"LoginData", Windows::Storage::ApplicationDataCreateDisposition::Always);
	if (loginData.Values().HasKey(L"RememberMe") && loginData.Values().Lookup(L"RememberMe").as<bool>())
	{
		RememberCheckBox().IsChecked(true);
	}
	if (loginData.Values().HasKey(L"AutoLogin") && loginData.Values().Lookup(L"AutoLogin").as<bool>())
	{
		AutoCheckBox().IsChecked(true);
	}
	if (settings.Values().HasKey(L"Theme"))
	{
		auto theme = Window::Current().Content().as<FrameworkElement>().RequestedTheme();
		if (settings.Values().Lookup(L"Theme").as<hstring>() == L"Light" && theme != ElementTheme::Dark)
		{
			Window::Current().Content().as<FrameworkElement>().RequestedTheme(ElementTheme::Light);
		}
		else if(settings.Values().Lookup(L"Theme").as<hstring>() == L"Dark" && theme != ElementTheme::Light)
		{
			Window::Current().Content().as<FrameworkElement>().RequestedTheme(ElementTheme::Dark);
		}
	}
	else
	{
		Window::Current().Content().as<FrameworkElement>().RequestedTheme(ElementTheme::Light);
		settings.Values().Insert(L"Theme", box_value(L"Light"));
	}
	if (m_emails.HasKey(L"Last"))
	{
		Email().Text(m_emails.GetNamedString(L"Last"));
	}
	if (RememberCheckBox().IsChecked().GetBoolean())
	{
		if (m_passwords.HasKey(Email().Text()))
		{
			Password().Password(m_passwords.GetNamedString(Email().Text()));
			if (AutoCheckBox().IsChecked().GetBoolean())
			{
				LoginClickHandler(sender, Windows::UI::Xaml::RoutedEventArgs{ nullptr });
			}
		}
	}
	else
	{
		Password().Password(L"");
	}
}
/// <summary>
/// 删除搜索记录
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void winrt::bikabika::implementation::MainPage::KeywordClose_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& /*e*/)
{
	auto border = sender.as<AppBarButton>().Parent().as<Grid>().Children().GetAt(0).as<Border>();
	auto grid = border.Child().as<Grid>();
	hstring keyword = ToolTipService::GetToolTip(border).as<ToolTip>().Content().as<hstring>();
	hstring keywordType = grid.Children().GetAt(1).as<TextBlock>().Text();
	for (uint32_t kIndex = 0; kIndex < m_suggestions.Size(); kIndex++)
	{
		auto x = m_suggestions.GetAt(kIndex);
		if (x.Keyword() == keyword && x.KeywordType() == keywordType)
		{
			m_suggestions.RemoveAt(kIndex);
			if (keywordType == resourceLoader.GetString(L"Keyword/SearchHistory"))
			{
				Windows::Storage::ApplicationDataContainer historys = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"Historys", Windows::Storage::ApplicationDataCreateDisposition::Always);
				JsonArray searchHistorys = Windows::Data::Json::JsonArray::Parse(historys.Values().Lookup(L"Search").as<hstring>());
				searchHistorys.RemoveAt(kIndex);
				historys.Values().Insert(L"Search", box_value(searchHistorys.Stringify()));
			}
		}
	}
	CatSearch().ItemsSource(box_value(m_suggestions));
}
/// <summary>
/// 自动转换省略号
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void winrt::bikabika::implementation::MainPage::Omit_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& /*e*/)
{
	TextBlock block = sender.as<TextBlock>();
	if (block.ActualWidth() > block.Width())
	{
		block.Text(Omit(block.Text(),6));
	}
}




/// <summary>
/// 修改签名回车响应
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void winrt::bikabika::implementation::MainPage::ChangeSignatureBox_KeyUp(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::KeyRoutedEventArgs const& e)
{
	if (e.Key() == Windows::System::VirtualKey::Enter)
	{
		ChangeSignatureButton_Click(sender, Windows::UI::Xaml::RoutedEventArgs{ nullptr });
	}
}


/// <summary>
/// 修改密码按钮响应
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
/// <returns></returns>
Windows::Foundation::IAsyncAction winrt::bikabika::implementation::MainPage::ChangePasswordButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
	if (OldPasswrod().Text() != L"" && NewPasswrod().Text() != L"")
	{
		Microsoft::UI::Xaml::Controls::ProgressRing ring;
		ring.IsActive(true);
		ring.Width(16);
		ring.Height(16);
		ChangePasswordButton().Content(ring);
		ChangePasswordButton().IsEnabled(false);
		auto res = co_await m_bikaClient.SetPassword(OldPasswrod().Text(), NewPasswrod().Text());
		if (res.Code() == -1)
		{
			ContentDialogShow(BikaHttpStatus::TIMEOUT, L"");
		}
		else if (res.Code() == 200)
		{
			InfoBarMessageShow(resourceLoader.GetString(L"FlyoutChangePassword/Text"), resourceLoader.GetString(L"Message/Change/Success"), Microsoft::UI::Xaml::Controls::InfoBarSeverity::Success);
			m_passwords.Insert(m_user.Email(), JsonValue::CreateStringValue(NewPasswrod().Text()));

			Windows::Storage::ApplicationDataContainer loginData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"LoginData", Windows::Storage::ApplicationDataCreateDisposition::Always);
			loginData.Values().Insert(L"passwords", box_value(m_passwords.Stringify()));
			ChangePassword(false);
		}
		else if (res.Code() == 401 && res.Error() == L"1005")
		{
			ContentDialogShow(BikaHttpStatus::NOAUTH, res.Message());
		}
		else
		{
			ContentDialogShow(BikaHttpStatus::UNKNOWN, res.Message());
		}
		ChangePasswordButton_Loaded(sender, Windows::UI::Xaml::RoutedEventArgs{ nullptr });

	}
}

/// <summary>
/// 修改签名按钮响应
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
/// <returns></returns>
Windows::Foundation::IAsyncAction winrt::bikabika::implementation::MainPage::ChangeSignatureButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
	if (NewSignature().Text() != L"")
	{
		Microsoft::UI::Xaml::Controls::ProgressRing ring;
		ring.IsActive(true);
		ring.Width(16);
		ring.Height(16);
		ChangeSignatureButton().Content(ring);
		ChangeSignatureButton().IsEnabled(false);
		auto res = co_await m_bikaClient.SetSlogan(NewSignature().Text());

		if (res.Code() == -1)
		{
			ContentDialogShow(BikaHttpStatus::TIMEOUT, L"");
		}
		else if (res.Code() == 200)
		{
			ChangeSignature(false);
			InfoBarMessageShow(resourceLoader.GetString(L"FlyoutChangeSignature/Text"), resourceLoader.GetString(L"Message/Change/Success"), Microsoft::UI::Xaml::Controls::InfoBarSeverity::Success);
			co_await SetPerson();
		}
		else if (res.Code() == 401 && res.Error() == L"1005")
		{
			ContentDialogShow(BikaHttpStatus::NOAUTH, res.Message());
		}
		else
		{
			ContentDialogShow(BikaHttpStatus::UNKNOWN, res.Message());
		}
		ChangeSignatureButton_Loaded(sender, Windows::UI::Xaml::RoutedEventArgs{ nullptr });
	}
}




void winrt::bikabika::implementation::MainPage::ButtonRegister_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
	Register(true);
}


void winrt::bikabika::implementation::MainPage::LoginUserSlogan_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{


}


Windows::Foundation::IAsyncAction winrt::bikabika::implementation::MainPage::ButtonChangeTitle_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
	if (NewTitle().Text() != L"")
	{
		Microsoft::UI::Xaml::Controls::ProgressRing ring;
		ring.IsActive(true);
		ring.Width(16);
		ring.Height(16);
		ChangeTitleButton().Content(ring);
		ChangeTitleButton().IsEnabled(false);
		auto res = co_await m_bikaClient.SetTitle(m_user.ID(), NewTitle().Text());
		if (res.Code() == -1)
		{
			ContentDialogShow(BikaHttpStatus::TIMEOUT, L"");
		}
		else if (res.Code() == 200)
		{
			ChangeTitle(false);
			InfoBarMessageShow(resourceLoader.GetString(L"FlyoutChangeTitle/Text"), resourceLoader.GetString(L"Message/Change/Success"), Microsoft::UI::Xaml::Controls::InfoBarSeverity::Success);
			co_await SetPerson();
		}
		else if (res.Code() == 401 && res.Error() == L"1005")
		{
			ContentDialogShow(BikaHttpStatus::NOAUTH, res.Message());
		}else if (res.Code() == 400 && res.Error() == L"1015")
		{
			ContentDialogShow(BikaHttpStatus::UNKNOWN, resourceLoader.GetString(L"FailMessage/Message/AuthError"));
		}
		else
		{
			ContentDialogShow(BikaHttpStatus::UNKNOWN, res.Message());
		}
		ChangeTitleButton_Loaded(sender, Windows::UI::Xaml::RoutedEventArgs{ nullptr });
	}
}


void winrt::bikabika::implementation::MainPage::ChangeTitle_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
	ChangeTitle(true);
}


void winrt::bikabika::implementation::MainPage::NewTitle_KeyUp(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::KeyRoutedEventArgs const& e)
{
	if (e.Key() == Windows::System::VirtualKey::Enter)
	{
		ButtonChangeTitle_Click(sender, Windows::UI::Xaml::RoutedEventArgs{ nullptr });
	}
}


void winrt::bikabika::implementation::MainPage::ChangeSignatureButton_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
	NewSignature().PlaceholderText(m_user.Slogan());
	ChangeSignatureButton().Content(box_value(resourceLoader.GetString(L"FlyoutChangeSignature/Text")));
	ChangeSignatureButton().IsEnabled(true);
}


void winrt::bikabika::implementation::MainPage::ChangePasswordButton_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
	ChangePasswordButton().Content(box_value(resourceLoader.GetString(L"FlyoutChangePassword/Text")));
	ChangePasswordButton().IsEnabled(true);
}




void winrt::bikabika::implementation::MainPage::ChangeTitleButton_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
	ChangeTitleButton().Content(box_value(resourceLoader.GetString(L"FlyoutChangeTitle/Text")));
	ChangeTitleButton().IsEnabled(true);
}


void winrt::bikabika::implementation::MainPage::BirthdayDatePicker_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
	std::time_t timep;
	time(&timep);
	std::tm* m = std::gmtime(&timep);
	m->tm_year -= 18;
	// 必须大于十八
	sender.as<CalendarDatePicker>().MaxDate(clock::from_time_t(mktime(m)));
	m->tm_mon -= 1;
	sender.as<CalendarDatePicker>().Date(clock::from_time_t(mktime(m)));
	m->tm_year = 0;
	m->tm_mon = 0;
	m->tm_mday = 0;
	sender.as<CalendarDatePicker>().MinDate(clock::from_time_t(mktime(m)));
	RegisterButton().Content(box_value(resourceLoader.GetString(L"TipRegister/Content")));
	RegisterButton().IsEnabled(true);
	GenderComboBox().ItemsSource(winrt::single_threaded_observable_vector<hstring>({ resourceLoader.GetString(L"Keyword/Gender/bot"),resourceLoader.GetString(L"Keyword/Gender/f") ,resourceLoader.GetString(L"Keyword/Gender/m") }));
	GenderComboBox().SelectedIndex(0);
}


Windows::Foundation::IAsyncAction  winrt::bikabika::implementation::MainPage::RegisterButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
	LoginViewShow(false);
	Microsoft::UI::Xaml::Controls::ProgressRing ring;
	ring.IsActive(true);
	ring.Width(16);
	ring.Height(16);
	LoginButton().Content(ring);
	RegisterButton().IsEnabled(false);
	time_t times = winrt::clock::to_time_t(BirthdayDatePicker().Date().as<DateTime>());
	std::wstringstream wss;
	wss << std::put_time(std::localtime(&times), L"%Y-%m-%d");
	BikaClient::Utils::Gender gender;
	if (GenderComboBox().SelectedIndex() == 0)
	{
		gender = BikaClient::Utils::Gender::Bot;
	}
	else if (GenderComboBox().SelectedIndex() == 1)
	{
		gender = BikaClient::Utils::Gender::Female;
	}
	else gender = BikaClient::Utils::Gender::Male;
	// 不允许为空
	if (Question1Box().Text() == L""
		|| Answer1Box().Text() == L""
		|| Question2Box().Text() == L""
		|| Answer2Box().Text() == L""
		|| Question3Box().Text() == L""
		|| Answer3Box().Text() == L""
		|| RegisterEmail().Text() == L""
		|| RegisterPassword().Password() == L""
		|| RegisterName().Text() == L"")
	{
		ContentDialogShow(bikabika::BikaHttpStatus::BLANK, L"");
		RegisterButton().Content(box_value(resourceLoader.GetString(L"TipRegister/Content")));
		RegisterButton().IsEnabled(true);
		return;
	}
	// 密码是否正确
	if (RegisterPassword().Password() == L""
		|| RegisterPasswordSure().Password() == L""
		|| RegisterPassword().Password() != RegisterPasswordSure().Password())
	{
		ContentDialogShow(bikabika::BikaHttpStatus::SUREERROR, L"");
		RegisterButton().Content(box_value(resourceLoader.GetString(L"TipRegister/Content")));
		RegisterButton().IsEnabled(true);
		return;
	}
	// 密码大于8
	if (RegisterPassword().Password().size() < 8)
	{
		ContentDialogShow(bikabika::BikaHttpStatus::PASSWORDTOOSHORT, L"");
		RegisterButton().Content(box_value(resourceLoader.GetString(L"TipRegister/Content")));
		RegisterButton().IsEnabled(true);
		return;
	}
	// 昵称[2,50]
	if (RegisterName().Text().size() < 2|| RegisterName().Text().size() > 50)
	{
		ContentDialogShow(bikabika::BikaHttpStatus::UNKNOWN, resourceLoader.GetString(L"FailMessage/Message/NameTooLong"));
		RegisterButton().Content(box_value(resourceLoader.GetString(L"TipRegister/Content")));
		RegisterButton().IsEnabled(true);
		return;
	}
	auto res = co_await m_bikaClient.Register(RegisterEmail().Text(), RegisterPassword().Password(), RegisterName().Text(), winrt::hstring{ wss.str() }, gender,Question1Box().Text(),Answer1Box().Text(),Question2Box().Text(),Answer2Box().Text(),Question3Box().Text(),Answer3Box().Text());
	if (res.Code() == -1)
	{
		ContentDialogShow(BikaHttpStatus::TIMEOUT, L"");
	}
	else if (res.Code() == 200)
	{
		Windows::Storage::ApplicationDataContainer loginData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"LoginData", Windows::Storage::ApplicationDataCreateDisposition::Always);
		auto emailArray = m_emails.GetNamedArray(L"emailArray");
		m_emails.Insert(L"Last", JsonValue::CreateStringValue(RegisterEmail().Text()));
		emailArray.Append(JsonValue::CreateStringValue(RegisterEmail().Text()));
		m_emails.Insert(L"emailArray", emailArray);
		loginData.Values().Insert(L"emails", box_value(m_emails.Stringify()));
		m_passwords.Insert(RegisterEmail().Text(), JsonValue::CreateStringValue(RegisterPassword().Password()));
		loginData.Values().Insert(L"passwords", box_value(m_passwords.Stringify()));
		Register(false);
		InfoBarMessageShow(resourceLoader.GetString(L"Keyword/Message/Register"), L"", Microsoft::UI::Xaml::Controls::InfoBarSeverity::Success);
		LoginViewShow(true);
	}
	else if (res.Code() == 401 && res.Error() == L"1005")
	{
		ContentDialogShow(BikaHttpStatus::NOAUTH, res.Message());
	}
	else
	{
		ContentDialogShow(BikaHttpStatus::UNKNOWN, res.Message());
	}
	RegisterButton().Content(box_value(resourceLoader.GetString(L"TipRegister/Content")));
	RegisterButton().IsEnabled(true);
}

void winrt::bikabika::implementation::MainPage::RegisterEmail_TextChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::TextBoxTextChangingEventArgs const& e)
{
	string str;
	for (auto x : to_string(sender.as<TextBox>().Text()))
	{
		if (x >= '0' && x <= '9' || x >= 'a' && x <= 'z' || x == '.' || x == '_')
		{
			str += x;
		}
	}
	sender.as<TextBox>().Text(to_hstring(str));
}


void winrt::bikabika::implementation::MainPage::Border_PointerPressed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
	if (m_loginUsers > 0) {
		SelectUserTip().Target(sender.as<FrameworkElement>());
		SelectUserTip().IsOpen(true);
	}
}


void winrt::bikabika::implementation::MainPage::StackPanel_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
	if (m_emails.HasKey(L"Last"))
	{
		JsonArray emailArray = m_emails.GetNamedArray(L"emailArray");
		if (emailArray.Size() > 0)
		{
			for (auto x : emailArray)
			{
				hstring email = x.GetString();
				for (uint32_t i = 0; i < m_loginUsers.Size(); i++)
				{
					if (m_loginUsers.GetAt(i).Email() == email)
					{
						m_loginUsers.RemoveAt(i);
						break;
					}
				}
				if (m_userDatas.HasKey(email))
				{
					m_loginUsers.Append(UserBlock(m_userDatas.GetNamedObject(email), m_bikaClient.FileServer()));
				}
			}
		}
	}

	if (m_emails.HasKey(L"Last")&&m_userDatas.HasKey(m_emails.GetNamedString(L"Last")))
	{
		BikaClient::Blocks::UserBlock userBlock(m_userDatas.GetNamedObject(m_emails.GetNamedString(L"Last")), m_bikaClient.FileServer());
		LoginUserName().Text(userBlock.Name());
		LoginUserLevel().Text(userBlock.LevelString());
		LoginUserTitle().Text(userBlock.Title());
		LoginUserPic().ProfilePicture(userBlock.Thumb().Img());
		hstring slogan = userBlock.Slogan();
		LoginUserSloganTip().Content(box_value(slogan));
		LoginUserSlogan().Text(Omit(userBlock.Slogan(), 11));
		for (auto x : LoginGridView().Items())
		{
			if (m_emails.GetNamedString(L"Last") == x.as<UserBlock>().Email())
			{
				LoginGridView().SelectedItem(x);
			}
		}
	}
	else
	{
		LoginUserName().Text(resourceLoader.GetString(L"Keyword/Default/Name"));
		LoginUserLevel().Text(resourceLoader.GetString(L"Keyword/Default/Level"));
		LoginUserTitle().Text(resourceLoader.GetString(L"Keyword/Default/Title"));
		hstring slogan = resourceLoader.GetString(L"Keyword/Default/Slogan");
		LoginUserSlogan().Text(slogan);
		LoginUserSloganTip().Content(box_value(slogan));
		LoginUserPic().ProfilePicture(BitmapImage{ Uri{ L"ms-appx:///Assets//Picacgs//placeholder_avatar_2.png" } });
	}
}


void winrt::bikabika::implementation::MainPage::LoginGridView_ItemClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::ItemClickEventArgs const& e)
{
	auto user = e.ClickedItem().as<UserBlock>();
	m_emails.SetNamedValue(L"Last", JsonValue::CreateStringValue(user.Email()));
	Windows::Storage::ApplicationDataContainer loginData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"LoginData", Windows::Storage::ApplicationDataCreateDisposition::Always);
	loginData.Values().Insert(L"emails", box_value(m_emails.Stringify()));
	OutputDebugStringW(user.Email().c_str());
	Email().Text(user.Email());
	if (m_passwords.HasKey(Email().Text()))
	{
		Password().Password(m_passwords.GetNamedString(Email().Text()));
	}
}


void winrt::bikabika::implementation::MainPage::Email_TextChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::TextChangedEventArgs const& e)
{
	JsonArray emailArray = m_emails.GetNamedArray(L"emailArray");
	hstring text = sender.as<TextBox>().Text();
	if (emailArray.Size() > 0)
	{
		bool f = false;
		for (auto x : emailArray)
		{
			if (x.GetString() == text && m_userDatas.HasKey(text))
			{
				f = true;

			}
		}
		if (f)
		{
			BikaClient::Blocks::UserBlock userBlock(m_userDatas.GetNamedObject(text), m_bikaClient.FileServer());
			if (userBlock.Name() != LoginUserName().Text())
			{
				LoginUserName().Text(userBlock.Name());
				LoginUserLevel().Text(userBlock.LevelString());
				LoginUserTitle().Text(userBlock.Title());
				LoginUserPic().ProfilePicture(userBlock.Thumb().Img());
				hstring slogan = userBlock.Slogan();
				LoginUserSloganTip().Content(box_value(slogan));
				LoginUserSlogan().Text(Omit(userBlock.Slogan(), 11));
			}
		}
		else if (LoginUserName().Text() != resourceLoader.GetString(L"Keyword/Default/Name"))
		{
			LoginUserName().Text(resourceLoader.GetString(L"Keyword/Default/Name"));
			LoginUserLevel().Text(resourceLoader.GetString(L"Keyword/Default/Level"));
			LoginUserTitle().Text(resourceLoader.GetString(L"Keyword/Default/Title"));
			hstring slogan = resourceLoader.GetString(L"Keyword/Default/Slogan");
			LoginUserSlogan().Text(slogan);
			LoginUserSloganTip().Content(box_value(slogan));
			LoginUserPic().ProfilePicture(BitmapImage{ Uri{ L"ms-appx:///Assets//Picacgs//placeholder_avatar_2.png" } });
		}
		for (auto x : LoginGridView().Items())
		{
			if (text == x.as<UserBlock>().Email())
			{
				LoginGridView().SelectedItem(x);
				return;
			}
		}
	}
	string str;
	for (auto x : to_string(sender.as<TextBox>().Text()))
	{
		if (x >= '0' && x <= '9' || x >= 'a' && x <= 'z' || x == '.' || x == '_')
		{
			str += x;
		}
	}
	sender.as<TextBox>().Text(to_hstring(str));
}


void winrt::bikabika::implementation::MainPage::Border_PointerPressed_1(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
	PointerPoint currentPoint = e.GetCurrentPoint(NULL);

	PointerPointProperties props = currentPoint.Properties();
	if (props.IsRightButtonPressed()) {
		auto flyout = Windows::UI::Xaml::Controls::Primitives::FlyoutBase::GetAttachedFlyout(sender.as<FrameworkElement>());
		auto options = Windows::UI::Xaml::Controls::Primitives::FlyoutShowOptions();

		flyout.ShowAt(sender.as<FrameworkElement>(), options);
	}

}


void winrt::bikabika::implementation::MainPage::AppBarButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
	UserBlock user = sender.as<AppBarButton>().Tag().as<UserBlock>();

	uint32_t i,j;
	if (m_loginUsers.IndexOf(user, i))
	{
		m_loginUsers.RemoveAt(i);
		auto emailArray = m_emails.GetNamedArray(L"emailArray");
		for (j = 0; j < emailArray.Size(); j++)
		{
			if (emailArray.GetStringAt(j) == user.Email())
			{
				emailArray.RemoveAt(j);
				break;
			}
		}
		if (m_passwords.HasKey(user.Email()))  m_passwords.Remove(user.Email());
		if (m_userDatas.HasKey(user.Email()))  m_userDatas.Remove(user.Email());
		if (m_emails.HasKey(L"Last") && m_emails.GetNamedString(L"Last") == user.Email())
		{
			Email().Text(L"");
			m_emails.Remove(L"Last");
		}

		Windows::Storage::ApplicationDataContainer loginData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"LoginData", Windows::Storage::ApplicationDataCreateDisposition::Always);
		m_emails.SetNamedValue(L"emailArray", emailArray);
		OutputDebugStringW(m_emails.Stringify().c_str());
		loginData.Values().Insert(L"emails", box_value(m_emails.Stringify()));
		loginData.Values().Insert(L"passwords", box_value(m_passwords.Stringify()));
		loginData.Values().Insert(L"userDatas", box_value(m_userDatas.Stringify()));
	}
}




IAsyncAction winrt::bikabika::implementation::MainPage::ChangeAvatar_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
	FileOpenPicker openPicker;
	openPicker.ViewMode(PickerViewMode::Thumbnail);
	openPicker.SuggestedStartLocation(PickerLocationId::PicturesLibrary);
	openPicker.FileTypeFilter().ReplaceAll({ L".jpg", L".jpeg", L".png" });
	StorageFile file = co_await openPicker.PickSingleFileAsync();
	if (file != nullptr)
	{
		Windows::Storage::Streams::IBuffer buffer{ co_await Windows::Storage::FileIO::ReadBufferAsync(file) };

		auto res = co_await m_bikaClient.SetAvatar(buffer, to_hstring(to_string(file.FileType()).substr(1)));
		if (res.Code() == 200)
		{
			co_await SetPerson();
			InfoBarMessageShow(resourceLoader.GetString(L"Message/Success/Avatar"), L"", Microsoft::UI::Xaml::Controls::InfoBarSeverity::Success);
		}
		else
		{
			ContentDialogShow(BikaHttpStatus::UNKNOWN, res.Message() + L":"+ res.Detail());
		}
	}
}
