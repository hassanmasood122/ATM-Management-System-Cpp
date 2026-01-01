
#include <SFML/Graphics.hpp>
#include <string>
#include <cctype>

enum Screen
{
    SPLASH,
    LOGIN,
    WELCOME,
    MENU,
    WITHDRAW,
    DEPOSIT,
    BALANCE,
    RECEIPT,
    LOCKED
};

struct Account
{
    std::string name;
    std::string pin;
    int balance;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(850, 520), "ATM Application");
    window.setFramerateLimit(60);

    // ---------- ICON ----------
    sf::Image icon;
    if (icon.loadFromFile("icon.png"))
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // ---------- FONT ----------
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
        return -1;

    // ---------- USERS (AUTO SET) ----------
    Account accounts[5] = {
        {"Ali",    "1111", 50000},
        {"AM",  "2222", 40000},
        {"Hassan", "3333", 60000},
        {"Sara",   "4444", 35000},
        {"Ayesha", "5555", 45000}
    };

    int currentUser = -1;
    int wrongAttempts = 0;

    // ---------- STATE ----------
    Screen currentScreen = SPLASH;

    std::string pinInput, amountInput;
    int lastAmount = 0;
    std::string lastType;

    sf::Clock splashClock, receiptClock, welcomeClock;

    // ---------- COLORS ----------
    sf::Color bg(25, 25, 25), btn(60, 130, 210), txt(230, 230, 230);

    // ---------- SPLASH ----------
    sf::Texture logoTexture;
    logoTexture.loadFromFile("logo.png");
    sf::Sprite logo(logoTexture);
    logo.setPosition(325, 120);

    sf::RectangleShape loadBg({ 400, 15 });
    loadBg.setPosition(225, 300);
    loadBg.setFillColor(sf::Color(60, 60, 60));

    sf::RectangleShape loadBar({ 0, 15 });
    loadBar.setPosition(225, 300);
    loadBar.setFillColor(sf::Color::Green);

    // ---------- TEXT ----------
    sf::Text title("ATM MACHINE", font, 34);
    title.setPosition(290, 25);
    title.setFillColor(txt);

    sf::Text display("", font, 22);
    display.setPosition(260, 140);
    display.setFillColor(txt);

    sf::Text input("", font, 22);
    input.setPosition(260, 180);
    input.setFillColor(sf::Color::Cyan);

    sf::Text info("", font, 28);
    info.setPosition(180, 240);

    sf::Text receipt("", font, 24);
    receipt.setPosition(200, 170);
    receipt.setFillColor(sf::Color::Green);

    // ---------- BUTTONS ----------
    auto makeBtn = [&](float y, const std::string& s)
        {
            sf::RectangleShape r({ 230,45 });
            r.setPosition(310, y);
            r.setFillColor(btn);
            sf::Text t(s, font, 20);
            t.setPosition(340, y + 10);
            t.setFillColor(sf::Color::White);
            return std::make_pair(r, t);
        };

    auto b1 = makeBtn(240, "1. Withdraw");
    auto b2 = makeBtn(295, "2. Deposit");
    auto b3 = makeBtn(350, "3. Check Balance");
    auto b4 = makeBtn(405, "4. Logout");
    auto back = makeBtn(350, "Back");

    // ================= MAIN LOOP =================
    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();

            // ---------- PIN / AMOUNT INPUT ----------
            if (e.type == sf::Event::TextEntered &&
                (currentScreen == LOGIN || currentScreen == WITHDRAW || currentScreen == DEPOSIT))
            {
                if (e.text.unicode == 8)
                {
                    if (currentScreen == LOGIN && !pinInput.empty()) pinInput.pop_back();
                    if (currentScreen != LOGIN && !amountInput.empty()) amountInput.pop_back();
                }
                else if (isdigit(e.text.unicode))
                {
                    if (currentScreen == LOGIN && pinInput.size() < 4) pinInput += char(e.text.unicode);
                    if (currentScreen != LOGIN) amountInput += char(e.text.unicode);
                }
            }

            // ---------- ENTER KEY ----------
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter)
            {
                // LOGIN
                if (currentScreen == LOGIN)
                {
                    currentUser = -1;
                    for (int i = 0; i < 5; i++)
                        if (pinInput == accounts[i].pin)
                            currentUser = i;

                    if (currentUser != -1)
                    {
                        wrongAttempts = 0;
                        pinInput.clear();
                        currentScreen = WELCOME;
                        welcomeClock.restart();
                    }
                    else
                    {
                        wrongAttempts++;
                        pinInput.clear();
                        if (wrongAttempts >= 3)
                            currentScreen = LOCKED;
                    }
                }

                // WITHDRAW
                else if (currentScreen == WITHDRAW && !amountInput.empty())
                {
                    lastAmount = std::stoi(amountInput);
                    if (lastAmount <= accounts[currentUser].balance)
                    {
                        accounts[currentUser].balance -= lastAmount;
                        lastType = "Withdraw";
                        currentScreen = RECEIPT;
                        receiptClock.restart();
                    }
                    amountInput.clear();
                }

                // DEPOSIT
                else if (currentScreen == DEPOSIT && !amountInput.empty())
                {
                    lastAmount = std::stoi(amountInput);
                    accounts[currentUser].balance += lastAmount;
                    lastType = "Deposit";
                    currentScreen = RECEIPT;
                    receiptClock.restart();
                    amountInput.clear();
                }
            }

            // ---------- MENU SHORTCUTS ----------
            if (e.type == sf::Event::KeyPressed && currentScreen == MENU)
            {
                if (e.key.code == sf::Keyboard::Num1) currentScreen = WITHDRAW;
                if (e.key.code == sf::Keyboard::Num2) currentScreen = DEPOSIT;
                if (e.key.code == sf::Keyboard::Num3) currentScreen = BALANCE;
                if (e.key.code == sf::Keyboard::Num4) currentScreen = LOGIN;
            }

            // ---------- MOUSE ----------
            if (e.type == sf::Event::MouseButtonPressed)
            {
                auto m = sf::Mouse::getPosition(window);
                if (currentScreen == MENU)
                {
                    if (b1.first.getGlobalBounds().contains(m.x, m.y)) currentScreen = WITHDRAW;
                    if (b2.first.getGlobalBounds().contains(m.x, m.y)) currentScreen = DEPOSIT;
                    if (b3.first.getGlobalBounds().contains(m.x, m.y)) currentScreen = BALANCE;
                    if (b4.first.getGlobalBounds().contains(m.x, m.y)) currentScreen = LOGIN;
                }
                if (currentScreen == BALANCE && back.first.getGlobalBounds().contains(m.x, m.y))
                    currentScreen = MENU;
            }
        }

        // ---------- SPLASH ----------
        if (currentScreen == SPLASH)
        {
            float p = splashClock.getElapsedTime().asSeconds() / 3.0f;
            loadBar.setSize({ 400 * p, 15 });
            if (p >= 1) currentScreen = LOGIN;
        }

        // ---------- WELCOME ----------
        if (currentScreen == WELCOME &&
            welcomeClock.getElapsedTime().asSeconds() > 2.5f)
            currentScreen = MENU;

        // ---------- RECEIPT ----------
        if (currentScreen == RECEIPT &&
            receiptClock.getElapsedTime().asSeconds() > 3)
            currentScreen = MENU;

        // ---------- TEXT ----------
        if (currentScreen == LOGIN)
        {
            display.setString("Enter PIN:");
            input.setString(pinInput);
        }
        if (currentScreen == WELCOME)
        {
            display.setString("Welcome, " + accounts[currentUser].name + "!");
            input.setString("");
        }
        if (currentScreen == MENU)
        {
            display.setString("Select Option (1-4)");
            input.setString("");
        }
        if (currentScreen == WITHDRAW)
        {
            display.setString("Enter Withdraw Amount:");
            input.setString(amountInput);
        }
        if (currentScreen == DEPOSIT)
        {
            display.setString("Enter Deposit Amount:");
            input.setString(amountInput);
        }
        if (currentScreen == BALANCE)
        {
            display.setString("Balance: Rs " +
                std::to_string(accounts[currentUser].balance));
        }

        receipt.setString(
            "TRANSACTION RECEIPT\n\nUser: " + accounts[currentUser].name +
            "\nType: " + lastType +
            "\nAmount: Rs " + std::to_string(lastAmount) +
            "\nRemaining Balance: Rs " +
            std::to_string(accounts[currentUser].balance)
        );

        // ---------- DRAW ----------
        window.clear(bg);

        if (currentScreen == SPLASH)
        {
            window.draw(logo);
            window.draw(loadBg);
            window.draw(loadBar);
        }
        else if (currentScreen == RECEIPT)
        {
            window.draw(receipt);
        }
        else if (currentScreen == LOCKED)
        {
            info.setString("ACCOUNT LOCKED!\nRestart Application");
            info.setFillColor(sf::Color::Red);
            window.draw(info);
        }
        else
        {
            window.draw(title);
            window.draw(display);
            window.draw(input);

            if (currentScreen == MENU)
            {
                window.draw(b1.first); window.draw(b1.second);
                window.draw(b2.first); window.draw(b2.second);
                window.draw(b3.first); window.draw(b3.second);
                window.draw(b4.first); window.draw(b4.second);
            }
            if (currentScreen == BALANCE)
            {
                window.draw(back.first);
                window.draw(back.second);
            }
        }

        window.display();
    }

    return 0;
}

