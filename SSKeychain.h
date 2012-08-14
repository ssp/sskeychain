//
//  SSKeychain.h
//  SSToolkit
//
//  Created by Sam Soffes on 5/19/10.
//  Copyright (c) 2009-2011 Sam Soffes. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Security/Security.h>

/** Error codes that can be returned in NSError objects. */
typedef enum {
	/** No error. */
	SSKeychainErrorNone = noErr,
	
	/** Some of the arguments were invalid. */
	SSKeychainErrorBadArguments = -1001,
	
	/** There was no password. */
	SSKeychainErrorNoPassword = -1002,
	
	/** One or more parameters passed internally were not valid. */
	SSKeychainErrorInvalidParameter = errSecParam,
	
	/** Failed to allocate memory. */
	SSKeychainErrorFailedToAllocated = errSecAllocate,
	
	/** No trust results are available. */
	SSKeychainErrorNotAvailable = errSecNotAvailable,
	
	/** Authorization/Authentication failed. */
	SSKeychainErrorAuthorizationFailed = errSecAuthFailed,
	
	/** The item already exists. */
	SSKeychainErrorDuplicatedItem = errSecDuplicateItem,
	
	/** The item cannot be found.*/
	SSKeychainErrorNotFound = errSecItemNotFound,
	
	/** Interaction with the Security Server is not allowed. */
	SSKeychainErrorInteractionNotAllowed = errSecInteractionNotAllowed,
	
	/** Unable to decode the provided data. */
	SSKeychainErrorFailedToDecode = errSecDecode
} SSKeychainErrorCode;

extern NSString *const kSSKeychainErrorDomain;

/** Account name. */
extern NSString *const kSSKeychainAccountKey;

/**
 Time the item was created.
 
 The value will be a string.
 */
extern NSString *const kSSKeychainCreatedAtKey;

/** Item class. */
extern NSString *const kSSKeychainClassKey;

/** Item description. */
extern NSString *const kSSKeychainDescriptionKey;

/** Item label. */
extern NSString *const kSSKeychainLabelKey;

/** Time the item was last modified.
 
 The value will be a string.
 */
extern NSString *const kSSKeychainLastModifiedKey;

/** Where the item was created. */
extern NSString *const kSSKeychainWhereKey;

/**
 Simple wrapper for accessing accounts, getting passwords, setting passwords, and deleting passwords using the system
 Keychain on Mac OS X and iOS.
 
 This was originally inspired by EMKeychain and SDKeychain (both of which are now gone). Thanks to the authors.
 SSKeychain has since switched to a simpler implementation that was abstracted from [SSToolkit](http://sstoolk.it).
 */
@interface SSKeychain : NSObject

///-----------------------
/// @name Getting Accounts
///-----------------------

/**
 Returns an array containing all generic type Keychain accounts, or `nil` if the Keychain has no generic accounts.
 
 See the `NSString` constants declared in SSKeychain.h for a list of keys that can be used when accessing the
 dictionaries returned by this method.
 
 @return An array of dictionaries containing the generic type Keychain accounts, or `nil` if the Keychain doesn't 
 have any generic accounts. The order of the objects in the array isn't defined.
 
 @see allAccounts:
 @see allAccountsOfClass:error:
 */
+ (NSArray *)allAccounts;

/**
 Returns an array containing all generic type Keychain accounts, or `nil` if the Keychain has no generic accounts.
 
 See the `NSString` constants declared in SSKeychain.h for a list of keys that can be used when accessing the
 dictionaries returned by this method.
 
 @param error If accessing the accounts fails, upon return contains an error that describes the problem.
 
 @return An array of dictionaries containing the generic type Keychain accounts, or `nil` if the Keychain doesn't 
 have any generic accounts. The order of the objects in the array isn't defined.
  
 @see allAccounts
 @see allAccountsOfClass:error:
 */
+ (NSArray *)allAccounts:(NSError **)error;

/**
 Returns an array containing all generic type Keychain accounts for the given service, or `nil` if the Keychain 
 doesn't have any generic accounts for the given service.
 
 See the `NSString` constants declared in SSKeychain.h for a list of keys that can be used when accessing the
 dictionaries returned by this method.
 
 @param serviceName The service for which to return the corresponding accounts.
 
 @return An array of dictionaries containing the generic type Keychain accounts for the given `serviceName`, or 
 `nil` if the Keychain doesn't have any generic accounts for the given `serviceName`. The order of the objects 
 in the array isn't defined.
 
 @see accountsForService:error:
 @see accountsForService:ofClass:error:
 */
+ (NSArray *)accountsForService:(NSString *)serviceName;

/**
 Returns an array containing all generic type Keychain accounts for the given service, or `nil` if the Keychain 
 doesn't have any generic accounts for the given service.
 
 @param serviceName The service for which to return the corresponding accounts.
 
 @param error If accessing the accounts fails, upon return contains an error that describes the problem.
 
 @return An array of dictionaries containing the generic type Keychain accounts for the given `serviceName`, or 
 `nil` if the Keychain doesn't have any generic accounts for the given `serviceName`. The order of the objects 
 in the array isn't defined.
 
 @see accountsForService:
 @see accountsForService:ofClass:error:
 */
+ (NSArray *)accountsForService:(NSString *)serviceName error:(NSError **)error;

/**
 Returns an array containing the Keychain's accounts of the given class, or `nil` if the Keychain has no accounts
 of that class.
 
 See the `NSString` constants declared in SSKeychain.h for a list of keys that can be used when accessing the
 dictionaries returned by this method.
 
 @param serviceClass The class that the returned accounts need to have.

 @param error If accessing the accounts fails, upon return contains an error that describes the problem.
 
 @return An array of dictionaries containing the Keychain's accounts of class `serviceClass`, or `nil` if the 
 Keychain doesn't have any accounts of that class. The order of the objects in the array isn't defined.
 
 @see allAccounts:
 */
+ (NSArray *)allAccountsOfClass:(CFTypeRef)serviceClass error:(NSError **)error;

/**
 Returns an array containing the Keychain's accounts for a given service of the given class, or `nil` if the 
 Keychain doesn't have any accounts for the given service of the given class.
 
 @param serviceName The service for which to return the corresponding accounts.

 @param serviceClass The class that the returned accounts need to have.
 
 @param error If accessing the accounts fails, upon return contains an error that describes the problem.
 
 @return An array of dictionarires containing the Keychain's accounts for a given `serviceName` and `serviceClass`,
 or `nil` if the Keychain doesn't have any accounts for the given `serviceName` and `serviceClass`. The order of 
 the objects in the array isn't defined.
 
 @see accountsForService:error:
 */
+ (NSArray *)accountsForService:(NSString *)serviceName ofClass:(CFTypeRef)serviceClass error:(NSError **)error;


///------------------------
/// @name Getting Passwords
///------------------------

/**
 Returns a string containing the password for a given account and service of generic type, or `nil` if the Keychain
 doesn't have a password for the given parameters.
 
 @param serviceName The service for which to return the corresponding password.
 
 @param account The account for which to return the corresponding password.
 
 @return Returns a string containing the password for a given `account` and `serviceName` of generic type, or `nil`
 if the Keychain doesn't have a password for the given parameters.
 
 @see passwordForService:account:error:
 @see passwordForService:ofClass:account:error:
 */
+ (NSString *)passwordForService:(NSString *)serviceName account:(NSString *)account;

/**
 Returns a string containing the password for a given account and service of generic type, or `nil` if the Keychain
 doesn't have a password for the given parameters.
 
 @param serviceName The service for which to return the corresponding password.
 
 @param account The account for which to return the corresponding password.
 
 @param error If accessing the password fails, upon return contains an error that describes the problem.
 
 @return Returns a string containing the password for a given `account` and `serviceName` of generic type, or `nil`
 if the Keychain doesn't have a password for the given parameters.
 
 @see passwordForService:account:
 @see passwordForService:ofClass:account:error:
 */
+ (NSString *)passwordForService:(NSString *)serviceName account:(NSString *)account error:(NSError **)error;

/**
 Returns a string containing the password for a given account, service name and service class, or `nil` if the
 Keychain doesn't have a password for the given parameters.
 
 @param serviceName The service for which to return the corresponding password.
 
 @param serviceClass The class that the returned accounts need to have.
 
 @param account The account for which to return the corresponding password.
 
 @param error If accessing the password fails, upon return contains an error that describes the problem.
 
 @return Returns a string containing the password for a given `account`, `serviceName` and `serviceType`, or `nil`
 if the Keychain doesn't have a password for the given parameters.
 
 @see passwordForService:account:error:
 */
+ (NSString *)passwordForService:(NSString *)serviceName ofClass:(CFTypeRef)serviceClass account:(NSString *)account error:(NSError **)error;

/**
 Returns the password data for a given account and service of generic type, or `nil` if the Keychain doesn't
 have data for the given parameters.
 
 @param serviceName The service for which to return the corresponding password.
 
 @param account The account for which to return the corresponding password.
 
 @param error If accessing the password fails, upon return contains an error that describes the problem.
 
 @return Returns data for a string containing the password for a given `account` and `serviceName` of generic type,
 or `nil` if the Keychain doesn't have a password for the given parameters.
 
 @see passwordDataForService:account:error:
 @see passwordDataForService:ofClass:account:error:
 */
+ (NSData *)passwordDataForService:(NSString *)serviceName account:(NSString *)account;

/**
 Returns the password data for a given account and service of generic type, or `nil` if the Keychain doesn't
 have data for the given parameters.
 
 @param serviceName The service for which to return the corresponding password.
 
 @param account The account for which to return the corresponding password.
 
 @param error If accessing the password fails, upon return contains an error that describes the problem.
 
 @return Returns data for a string containing the password for a given `account` and `serviceName` of generic type,
 or `nil` if the Keychain doesn't have a password for the given parameters.
 
 @see passwordDataForService:account:
 @see passwordDataForService:ofClass:account:error:
 */
+ (NSData *)passwordDataForService:(NSString *)serviceName account:(NSString *)account error:(NSError **)error;

/**
 Returns the password data for a given accounts and service, or `nil` if the Keychain doesn't have data 
 for the given parameters.
 
 @param serviceName The service for which to return the corresponding password.
 
 @param serviceClass The class that the returned accounts need to have.
 
 @param account The account for which to return the corresponding password.

 @param error If accessing the password fails, upon return contains an error that describes the problem.
 
 @return Returns data for a string containing the password for a given `account`, `serviceName` and `serviceType`,
 or `nil` if the Keychain doesn't have a password for the given parameters.
 
 @see passwordDataForService:account:error:
 */
+ (NSData *)passwordDataForService:(NSString *)serviceName ofClass:(CFTypeRef)serviceClass account:(NSString *)account error:(NSError **)error;


///-------------------------
/// @name Deleting Passwords
///-------------------------

/**
 Deletes a password from the Keychain.
 
 @param serviceName The service for which to delete the corresponding password.
 
 @param account The account for which to delete the corresponding password.
 
 @return Returns `YES` on success, or `NO` on failure.
 
 @see deletePasswordForService:account:error:
 @see deletePasswordForService:ofClass:account:
 */
+ (BOOL)deletePasswordForService:(NSString *)serviceName account:(NSString *)account;

/**
 Deletes a password from the Keychain.
 
 @param serviceName The service for which to delete the corresponding password.
 
 @param account The account for which to delete the corresponding password.
 
 @param error If deleting the password fails, upon return contains an error that describes the problem.
 
 @return Returns `YES` on success, or `NO` on failure.
 
 @see deletePasswordForService:account:
 @see deletePasswordForService:ofClass:account:
 */
+ (BOOL)deletePasswordForService:(NSString *)serviceName account:(NSString *)account error:(NSError **)error;

/**
 Deletes a password from the Keychain.
 
 @param serviceName The service for which to delete the corresponding password.

 @param serviceClass The class that the returned accounts need to have.

 @param account The account for which to delete the corresponding password.
 
 @param error If deleting the password fails, upon return contains an error that describes the problem.
 
 @return Returns `YES` on success, or `NO` on failure.
 
 @see deletePasswordForService:account:error:
 */
+ (BOOL)deletePasswordForService:(NSString *)serviceName ofClass:(CFTypeRef)serviceClass account:(NSString *)account error:(NSError **)error;


///------------------------
/// @name Setting Passwords
///------------------------

/**
 Sets a password in the Keychain.
 
 @param password The password to store in the Keychain.
 
 @param serviceName The service for which to set the corresponding password.
 
 @param account The account for which to set the corresponding password.
 
 @return Returns `YES` on success, or `NO` on failure.
 
 @see setPassword:forService:account:error:
 @see setPassword:forService:ofClass:account:error:
 */
+ (BOOL)setPassword:(NSString *)password forService:(NSString *)serviceName account:(NSString *)account;

/**
 Sets a password in the Keychain.
 
 @param password The password to store in the Keychain.
 
 @param serviceName The service for which to set the corresponding password.
 
 @param account The account for which to set the corresponding password.
 
 @param error If setting the password fails, upon return contains an error that describes the problem.
 
 @return Returns `YES` on success, or `NO` on failure.
 
 @see setPassword:forService:account:
 @see setPassword:forService:ofClass:account:error:
 */
+ (BOOL)setPassword:(NSString *)password forService:(NSString *)serviceName account:(NSString *)account error:(NSError **)error;

/**
 Sets arbitrary data in the Keychain.
 
 @param password The data to store in the Keychain.
 
 @param serviceName The service for which to set the corresponding password.
 
 @param account The account for which to set the corresponding password.
 
 @param error If setting the password fails, upon return contains an error that describes the problem.
 
 @return Returns `YES` on success, or `NO` on failure.
 
 @see setPasswordData:forService:account:error:
 @see setPasswordData:forService:ofClass:account:error:
 */
+ (BOOL)setPasswordData:(NSData *)password forService:(NSString *)serviceName account:(NSString *)account;

/**
 Sets arbitrary data in the Keychain.
 
 @param password The data to store in the Keychain.
 
 @param serviceName The service for which to set the corresponding password.
 
 @param account The account for which to set the corresponding password.
 
 @param error If setting the password fails, upon return contains an error that describes the problem.
 
 @return Returns `YES` on success, or `NO` on failure.
 
 @see setPasswordData:forService:account:
 @see setPasswordData:forService:ofClass:account:error:
 */
+ (BOOL)setPasswordData:(NSData *)password forService:(NSString *)serviceName account:(NSString *)account error:(NSError **)error;

/**
 Sets a password in the Keychain.
 
 @param password The password to store in the Keychain.
 
 @param serviceName The service for which to set the corresponding password.

 @param serviceClass The class that the created Keychain entry will have.

 @param account The account for which to set the corresponding password.
 
 @param error If setting the password fails, upon return contains an error that describes the problem.
 
 @return Returns `YES` on success, or `NO` on failure.
 
 @see setPassword:forService:account:error:
 */
+ (BOOL)setPassword:(NSString *)password forService:(NSString *)serviceName ofClass:(CFTypeRef)serviceClass account:(NSString *)account error:(NSError **)error;

/**
 Sets arbitrary data in the Keychain.
 
 @param password The data to store in the Keychain.
 
 @param serviceName The service for which to set the corresponding password.
 
 @param serviceClass The class that the created Keychain entry will have.

 @param account The account for which to set the corresponding password.
 
 @param error If setting the password fails, upon return contains an error that describes the problem.
 
 @return Returns `YES` on success, or `NO` on failure.
 
 @see setPasswordData:forService:account:error:
 */
+ (BOOL)setPasswordData:(NSData *)password forService:(NSString *)serviceName ofClass:(CFTypeRef)serviceClass account:(NSString *)account error:(NSError **)error;


///--------------------
/// @name Configuration
///--------------------

#if __IPHONE_4_0 && TARGET_OS_IPHONE
/**
 Returns the accessibility type for all future passwords saved to the Keychain.
 
 @return Returns the accessibility type.
 
 The return value will be `NULL` or one of the "Keychain Item Accessibility Constants" used for determining when a
 keychain item should be readable.
 
 @see accessibilityType
 */
+ (CFTypeRef)accessibilityType;

/**
 Sets the accessibility type for all future passwords saved to the Keychain.
 
 @param accessibilityType One of the "Keychain Item Accessibility Constants" used for determining when a keychain item
 should be readable.
 
 If the value is `NULL` (the default), the Keychain default will be used.
 
 @see accessibilityType
 */
+ (void)setAccessibilityType:(CFTypeRef)accessibilityType;
#endif

@end
