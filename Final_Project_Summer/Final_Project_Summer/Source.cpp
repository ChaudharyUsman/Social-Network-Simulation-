#include <iostream>
#include <string>
using namespace std;

class UserProfile; // Forward declaration

class Comment // Class representing a comment on a post
{
public:
    int commentId;
    string text;
    Comment* parent;
    Comment* next;

    Comment(int id, const string& content) : commentId(id), text(content), parent(nullptr), next(nullptr) {}
};

class Post // Class representing a post
{
public:
    int postId;
    string content;
    UserProfile* author;
    Comment* comments;
    Post* next;
    int timestamp;

    Post(int id, const string& text, UserProfile* user)
        : postId(id), content(text), author(user),
        comments(nullptr), next(nullptr), timestamp(0) {}
};

class Notification // Class representing a notification
{
public:
    string content;
    Notification* next;

    Notification(const string& msg) : content(msg), next(nullptr) {}
};

class UserProfile // Class representing a user profile in the social network
{
private:
    Notification* notifications; // Linked list to store notifications

public:
    int userID;
    string username;
    string profileDetails;
    UserProfile* next;
    UserProfile** friends;
    int num_friends;
    Post* posts;
    Post* newsFeed;
    int postIdCounter; // Counter for assigning post IDs
    int commentIdCounter; // Counter for assigning comment IDs

    UserProfile(int id, const string& name, const string& details)
        : userID(id), username(name), profileDetails(details),
        next(nullptr), friends(nullptr), num_friends(0), posts(nullptr), newsFeed(nullptr),
        postIdCounter(1), commentIdCounter(1) {}

    void add_friend(UserProfile* friendProfile) // Adds a friend to the user's list of friends
    {
        UserProfile** newFriends = new UserProfile * [num_friends + 1];
        for (int i = 0; i < num_friends; ++i)
        {
            newFriends[i] = friends[i];
        }
        newFriends[num_friends] = friendProfile;
        delete[] friends;
        friends = newFriends;
        num_friends++;
    }

    bool check_friend_with(UserProfile* friendProfile) // Checks if a given user profile is a friend of the current user

    {
        for (int i = 0; i < num_friends; ++i)
        {
            if (friends[i] == friendProfile)
            {
                return true;
            }
        }
        return false;
    }

    void create_post(const string& content) // Creates a new post and adds it to the user's list of posts
    {
        Post* newPost = new Post(postIdCounter++, content, this);
        newPost->next = posts;
        posts = newPost;
    }

    void add_comment_to_Post(Post* post, const string& commentText)  // Adds a comment to a specific post
    {
        Comment* newComment = new Comment(commentIdCounter++, commentText);
        newComment->next = post->comments;
        post->comments = newComment;
    }

    void addReplyToComment(Comment* parentComment, const string& replyText) // Adds a reply to a specific commen
    {
        Comment* newReply = new Comment(commentIdCounter++, replyText);
        newReply->parent = parentComment;
        newReply->next = parentComment->next;
        parentComment->next = newReply;
    }

    void addToNewsFeed(Post* post) // Adds a post to the user's news feed
    {
        post->next = newsFeed;
        newsFeed = post;
    }

    void generateNewsFeed()  // Generates and displays the user's news feed
    {
        cout << "News Feed for " << username << ":" << endl;
        Post* currentPost = newsFeed;
        while (currentPost)
        {
            cout << currentPost->author->username << " posted: " << currentPost->content << endl;
            currentPost = currentPost->next;
        }
    }

    void addNotification(const string& notification)  // Adds a notification to the user's list of notifications
    {
        Notification* newNotification = new Notification(notification);
        newNotification->next = notifications;
        notifications = newNotification;
    }

    void viewNotifications() // Displays the user's list of notifications
    {
        cout << "Notifications for " << username << ":" << endl;
        Notification* currentNotification = notifications;
        while (currentNotification)
        {
            cout << currentNotification->content << endl;
            currentNotification = currentNotification->next;
        }
    }

    Post* getPostByID(int postId) // Retrieves a post by its ID from the user's list of posts
    {
        Post* currentPost = posts;
        while (currentPost)
        {
            if (currentPost->postId == postId)
            {
                return currentPost;
            }
            currentPost = currentPost->next;
        }
        return nullptr;
    }

    Comment* getCommentByID(int commentId) // Retrieves a comment by its ID from the user's list of posts and comments
    {
        Post* currentPost = posts;
        while (currentPost)
        {
            Comment* currentComment = currentPost->comments;
            while (currentComment)
            {
                if (currentComment->commentId == commentId)
                {
                    return currentComment;
                }
                currentComment = currentComment->next;
            }
            currentPost = currentPost->next;
        }
        return nullptr;
    }

    ~UserProfile()
    {
        delete[] friends; // Deallocate the friends array
    }
};

class SocialNetwork // Class representing the social network
{
private:
    UserProfile* head;

public:
    SocialNetwork() : head(nullptr) {}

    void addUser(int id, const string& name, const string& details)  // Adds a new user to the social network
    {
        UserProfile* newUser = new UserProfile(id, name, details);
        if (!head)
        {
            head = newUser;
        }
        else
        {
            newUser->next = head;
            head = newUser;
        }
    }

    UserProfile* getUserByID(int id) // Retrieves a user profile by its ID from the social network
    {
        UserProfile* current = head;
        while (current)
        {
            if (current->userID == id)
            {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void addFriendship(int id1, int id2) // Creates a friendship between two users
    {
        UserProfile* user1 = getUserByID(id1);
        UserProfile* user2 = getUserByID(id2);
        if (!user1 || !user2)
        {
            cout << "One or both users not found!" << endl;
            return;
        }

        user1->add_friend(user2);
        user2->add_friend(user1);
    }

    bool areFriends(int id1, int id2) // Checks if two users are friends
    {
        UserProfile* user1 = getUserByID(id1);
        UserProfile* user2 = getUserByID(id2);
        if (!user1 || !user2)
        {
            cout << "One or both users not found!" << endl;
            return false;
        }

        return user1->check_friend_with(user2);
    }

    void removeUser(int id) // Removes a user profile from the social network
    {
        UserProfile* current = head;
        UserProfile* prev = nullptr;

        while (current)
        {
            if (current->userID == id)
            {
                if (prev)
                {
                    prev->next = current->next;
                }
                else
                {
                    head = current->next;
                }

                delete current;
                cout << "User removed successfully." << endl;
                return;
            }

            prev = current;
            current = current->next;
        }

        cout << "User not found." << endl;
    }


    ~SocialNetwork() // Destructor for the SocialNetwork class
    {
        UserProfile* current = head;
        while (current)
        {
            UserProfile* next = current->next;
            delete current;
            current = next;
        }
    }
};

int main()
{

    SocialNetwork obj;

    int choice;
    do {
        cout << "Menu:" << endl;
        cout << "1. Add User" << endl;
        cout << "2. Add Friendship" << endl;
        cout << "3. Check Friendship" << endl;
        cout << "4. Display News Feed" << endl;
        cout << "5. Add Notification" << endl;
        cout << "6. View Notifications" << endl;
        cout << "7. Create Post" << endl;
        cout << "8. Add Comment to Post" << endl;
        cout << "9. Reply to Comment" << endl;
        cout << "10. user ID to remove: " << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int id;
            string name, details;
            cout << "Enter user ID: ";
            cin >> id;
            cout << "Enter username: ";
            cin.ignore(); // Ignore newline left in the buffer
            getline(cin, name);
            cout << "Enter profile details: ";
            getline(cin, details);
            obj.addUser(id, name, details);
            cout << "User added successfully." << endl;
            break;
        }
        case 2:
        {
            int id1, id2;
            cout << "Enter user IDs to create friendship (user1 user2): ";
            cin >> id1 >> id2;
            obj.addFriendship(id1, id2);
            cout << "Friendship added successfully." << endl;
            break;
        }
        case 3: {
            int id1, id2;
            cout << "Enter user IDs to check friendship (user1 user2): ";
            cin >> id1 >> id2;
            if (obj.areFriends(id1, id2))
            {
                cout << "Users " << id1 << " and " << id2 << " are friends." << endl;
            }
            else
            {
                cout << "Users " << id1 << " and " << id2 << " are not friends." << endl;
            }
            break;
        }
        case 4:
        {
            int id;
            cout << "Enter user ID to display news feed: ";
            cin >> id;
            UserProfile* user = obj.getUserByID(id);
            if (user)
            {
                user->generateNewsFeed();
            }
            else
            {
                cout << "User not found." << endl;
            }
            break;
        }
        case 5:
        {
            int id;
            string notification;
            cout << "Enter user ID to add notification: ";
            cin >> id;
            cout << "Enter notification message: ";
            cin.ignore();
            getline(cin, notification);
            UserProfile* user = obj.getUserByID(id);
            if (user)
            {
                user->addNotification(notification);
                cout << "Notification added successfully." << endl;
            }
            else
            {
                cout << "User not found." << endl;
            }
            break;
        }
        case 6:
        {
            int id;
            cout << "Enter user ID to view notifications: ";
            cin >> id;
            UserProfile* user = obj.getUserByID(id);
            if (user)
            {
                user->viewNotifications();
            }
            else
            {
                cout << "User not found." << endl;
            }
            break;
        }
        case 7:
        {
            int id;
            string content;
            cout << "Enter user ID to create a post: ";
            cin >> id;
            cout << "Enter post content: ";
            cin.ignore();
            getline(cin, content);
            UserProfile* user = obj.getUserByID(id);
            if (user)
            {
                user->create_post(content);
                cout << "Post created successfully." << endl;
            }
            else
            {
                cout << "User not found." << endl;
            }
            break;
        }
        case 8:
        {
            int id;
            int postId;
            string comment;
            cout << "Enter user ID to who's post you want to add a comment: ";
            cin >> id;
            cout << "Enter post ID to add a comment: ";
            cin >> postId;
            cout << "Enter comment: ";
            cin.ignore();
            getline(cin, comment);
            UserProfile* user = obj.getUserByID(id);
            Post* post = user->getPostByID(postId); // Implement this function
            if (post)
            {
                user->add_comment_to_Post(post, comment);
                cout << "Comment added successfully." << endl;
            }
            else
            {
                cout << "Post not found." << endl;
            }
            break;
        }
        case 9:
        {
            int id;
            int commentId;
            string reply;
            cout << "Enter user ID to who's post you want to add a reply to comment: ";
            cin >> id;
            cout << "Enter comment ID to reply: ";
            cin >> commentId;
            cout << "Enter reply: ";
            cin.ignore();
            getline(cin, reply);
            UserProfile* user = obj.getUserByID(id);
            Comment* comment = user->getCommentByID(commentId); // Implement this function
            if (comment)
            {

                user->addReplyToComment(comment, reply);
                cout << "Reply added successfully." << endl;
            }
            else
            {
                cout << "Comment not found." << endl;
            }
            break;
        }
        case 10:
        {
            int id;
            cout << "Enter user ID to remove: ";
            cin >> id;
            obj.removeUser(id);
            break;
        }
        case 0:
            cout << "Exiting." << endl;
            break;
        default:
            cout << "Invalid choice. Please enter a valid option." << endl;
        }

    } while (choice != 0);

    return 0;
}
